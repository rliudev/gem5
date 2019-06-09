/*
 * Author: Victor Fu
 *         Thomas Chang
 *         Rach Liu
 *         Daan Leiva
 */

#include "mem/cache/prefetch/perceptron_unit.hh"

#include "base/intmath.hh"
#include "base/logging.hh"

#include "params/PerceptronUnit.hh"

using AddrPriority = std::pair<Addr, int32_t>;  // Copied over from queued.hh

const int INST_SHIFT_AMT = 2; // Previously: param to BranchPredictor.py

PerceptronUnit::PerceptronUnit(const PerceptronUnitParams *p)
    : ClockedObject(p), perceptron_list_size(p->exponential_size),
      perceptron_size(p->perceptron_size), pf_timeout(p->pf_timeout),
      reject_all(p->reject_all), accept_all(p->accept_all)
{
  min_confidence = perceptron_size*2+14;

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

  // initialize prediction history list
  for(int i = 0; i < perceptron_list_size; i++)
  {
    prediction_history.push_back(-1);
  }

}

void PerceptronUnit::shouldPrefetch(std::vector<AddrPriority> &addresses)
{
  if (reject_all) {
    addresses.clear();
    return;
  }
  else if (accept_all) {
    return;
  }

  auto it = addresses.begin();
  while (it != addresses.end()) {
    bool shouldUse = lookup(it->first);
    if (!shouldUse) {
      it = addresses.erase(it);
    }
    else {
      ++it;
    }
  }

}


void PerceptronUnit::updateExpiredPfs() {
  while (hasTimedOutEntry()) {
    std::vector<Addr> timed_out_list = pf_timer_queue.back();
    pf_timer_queue.pop_back();
    for (auto exp_addr : timed_out_list) {
      update(exp_addr, false);
    }
  }
}


void PerceptronUnit::queuePfAddrs(const std::vector<Addr> addrs) {
  pf_timer_queue.insert(pf_timer_queue.begin(), addrs);
}


void PerceptronUnit::invalidatePfAddrs(Addr hitAddr) {
  for (auto it = pf_timer_queue.begin(); it != pf_timer_queue.end(); it++) {
    for (auto it2 = it->begin(); it2 != it->end(); ) {
      if ( *it2 == hitAddr) {
        it2 = it->erase(it2);
      } else {
        it2++;
      }
    }
  }
}


void PerceptronUnit::reset()
{
  // call the individual reset method for each perceptron
  for(int i = 0; i < perceptron_list_size; i++)
  {
    perceptron_list[i]->reset();
  }
}


bool PerceptronUnit::lookup(Addr pf_addr)
{
  // Original:
//  // find the index of the perceptron
//  int perceptron_index = (branch_addr >> instShiftAmt) & (perceptron_list_size - 1);
//  // grap the perceptron we need to calculate prediction
//  Perceptron* new_perceptron = perceptron_list[perceptron_index];
//  // generate elements needed for history struct
//  int perceptron_output = new_perceptron->predict(global_history);
//  // insert into history object
//  BPHistory *history = new BPHistory;
//  history->perceptron_output = perceptron_output;
//  history->global_history = global_history;
//  // update the referenced bp_history with the history we created
//  bp_history = static_cast<void*>(history);
//  // update our global history instance variable
//  global_history.insert(global_history.begin() + 1, ((perceptron_output >= 0)? 1 : -1));
//  global_history.pop_back();
//
//  return perceptron_output >= 0;


  // find the index of the perceptron
  //    orig: 2 was the instShiftAmount as required by BranchPredictor.py
//  printf("perceptron_list_size: %d\n", perceptron_list_size);
  int perceptron_index = (pf_addr >> INST_SHIFT_AMT) & (perceptron_list_size - 1);
//  printf("perceptron_index on lookup: %d\n", perceptron_index);
  // grap the perceptron we need to calculate prediction
  Perceptron* new_perceptron = perceptron_list[perceptron_index];
  // generate elements needed for history struct
  int perceptron_output = new_perceptron->predict(global_history);
  // store into prediction_history
  prediction_history[perceptron_index] = perceptron_output;
  // update our global history instance variable
  global_history.insert(global_history.begin() + 1, ((perceptron_output >= 0)? 1 : -1));
  global_history.pop_back();

  return perceptron_output >= 0;
}


void PerceptronUnit::update(Addr pf_addr, bool used)
{
  // Original:
  // we can only proceed if we have a history object
//  if(bp_history != NULL)
//  {
//    //cout << "entered bp_hitory NO NULL\n";
//    // cast the void pointer to our struct type
//    BPHistory *history = static_cast<BPHistory*>(bp_history);
//    int prev_branch_act = taken? 1 : -1;
//    if(squashed)
//    {
//      // if this path is being squashed we rever our global history
//      global_history = history->global_history;
//      global_history.insert(global_history.begin() + 1, prev_branch_act);
//      global_history.pop_back();
//      return;
//    }
//    // find the index of the perceptron
//    int perceptron_index = (branch_addr >> instShiftAmt) & (perceptron_list_size - 1);
//    // grab the perceptron we need to train
//    Perceptron* new_perceptron = perceptron_list[perceptron_index];
//    // train perceptron
//    new_perceptron->train(min_confidence, history->global_history, history->perceptron_output, prev_branch_act);
//    // delete unnecesary pointer
//    delete history;
//  }


  int actual_pf_act = used? 1 : -1;
  // find the index of the perceptron
  int perceptron_index = (pf_addr >> INST_SHIFT_AMT) & (perceptron_list_size - 1);
  // grab the perceptron we need to train
  Perceptron* new_perceptron = perceptron_list[perceptron_index];
  // train perceptron
  new_perceptron->train(min_confidence, global_history, prediction_history[perceptron_index], actual_pf_act);
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


PerceptronUnit *PerceptronUnitParams::create()
{
  return new PerceptronUnit(this);
}


