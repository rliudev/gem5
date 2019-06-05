/*
 * Author: Victor Fu
 *         Thomas Chang
 *         Rach Liu
 *         Daan Leiva
 */

#include "mem/cache/prefetch/perceptron_unit.hh"

#include "base/intmath.hh"
#include "base/logging.hh"

using AddrPriority = std::pair<Addr, int32_t>;  // Copied over from queued.hh

const int INST_SHIFT_AMT = 2; // Previously: param to BranchPredictor.py

//PerceptronUnit::PerceptronUnit(const PerceptronUnitParams *params)
PerceptronUnit::PerceptronUnit()
{
//  perceptron_list_size = params->exponential_size; // num of perceptrons in our system
//  perceptron_size = params->perceptron_size; // size of individual perceptrons
//  min_confidence = perceptron_size*2+14; // threshold to decide if updating a perceptron
  perceptron_list_size = 32;
  perceptron_size = 20;
  min_confidence = perceptron_size*2+14; // threshold to decide if updating a perceptron

  // in order to use lower bits as look up values we need to make sure the resulting
  // value is a power of 2
  if(!isPowerOf2(perceptron_list_size))
  {
    fatal("Invalid perceptron list size!\n");
  }

  // initialize our perceptron list
  for(int i = 0; i < perceptron_list_size; i++)
  {
    perceptron_list.push_back(new Perceptron(perceptron_size));
  }

  // initialize our global history list
  // the bias node (w_0) will need a weight of 1 so we instert that first
  global_history.push_back(1);
  for(int i = 1; i < perceptron_size; i++)
  {
    global_history.push_back(1);
  }
}

bool PerceptronUnit::shouldPrefetch(const BasePrefetcher::PrefetchInfo &pfi, std::vector<AddrPriority> &addresses)
{
  // Reject all
  int rejectAll = 1;
  if (rejectAll) {
    addresses.clear();
  }

  return 0;
}


void PerceptronUnit::reset()
{
  // call the individual reset method for each perceptron
  for(int i = 0; i < perceptron_list_size; i++)
  {
    perceptron_list[i]->reset();
  }
}

bool PerceptronUnit::lookup(Addr pf_addr, void *&pf_history)
{
  // find the index of the perceptron
  //    orig: 2 was the instShiftAmount as required by BranchPredictor.py
  int perceptron_index = (pf_addr >> INST_SHIFT_AMT) & (perceptron_list_size - 1);
  // grap the perceptron we need to calculate prediction
  Perceptron* new_perceptron = perceptron_list[perceptron_index];
  // generate elements needed for history struct
  int perceptron_output = new_perceptron->predict(global_history);
  // insert into history object
  PFHistory *history = new PFHistory;
  history->perceptron_output = perceptron_output;
  history->global_history = global_history;
  // update the referenced pf_history with the history we created
  pf_history = static_cast<void*>(history);
  // update our global history instance variable
  global_history.insert(global_history.begin() + 1, ((perceptron_output >= 0)? 1 : -1));
  global_history.pop_back();

  return perceptron_output >= 0;
}

void PerceptronUnit::update(Addr pf_addr, bool used, void *pf_history, bool squashed)
{
  // we can only proceed if we have a history object
  if(pf_history != NULL)
  {
    //cout << "entered bp_hitory NO NULL\n";
    // cast the void pointer to our struct type
    PFHistory *history = static_cast<PFHistory*>(pf_history);
    int prev_pf_act = used? 1 : -1;
    if(squashed)
    {
      // if this path is being squashed we rever our global history
      global_history = history->global_history;
      global_history.insert(global_history.begin() + 1, prev_pf_act);
      global_history.pop_back();
      return;
    }
    // find the index of the perceptron
    int perceptron_index = (pf_addr >> INST_SHIFT_AMT) & (perceptron_list_size - 1);
    // grab the perceptron we need to train
    Perceptron* new_perceptron = perceptron_list[perceptron_index];
    // train perceptron
    new_perceptron->train(min_confidence, history->global_history, history->perceptron_output, prev_pf_act);
    // delete unnecesary pointer
    delete history;
  }
}

void PerceptronUnit::squash(ThreadID tid, void *pf_history)
{
  // in order to get he deconstructors to run, we need to cast our history
  // before removing it
  PFHistory *history = static_cast<PFHistory*>(pf_history);
  // recover global history
  global_history = history->global_history;
  // delete element
  delete history;
}

void PerceptronUnit::uncondBranch(ThreadID tid, Addr pc, void *&pf_history)
{
//  // An unconditional pf is just a taken pf
//  PFHistory *history = new PFHistory;
//  history->perceptron_output =  1; // assume taken output
//  history->global_history = global_history;
//  pf_history = static_cast<void *>(history);
//  // update gobal history
//  global_history.insert(global_history.begin() + 1, 1);
//  global_history.pop_back();
}


