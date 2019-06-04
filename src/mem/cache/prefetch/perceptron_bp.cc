/*
 * Author: Daan Leiva
 */

#include "cpu/pred/perceptron_bp.hh"
#include "cpu/pred/perceptron.hh"

#include "base/intmath.hh"
#include "base/logging.hh"

PerceptronBP::PerceptronBP(const PerceptronBPParams *params) : BPredUnit(params)
{
  perceptron_list_size = params->exponential_size; // num of perceptrons in our system
  perceptron_size = params->perceptron_size; // size of individual perceptrons
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

void PerceptronBP::reset()
{
  // call the individual reset method for each perceptron
  for(int i = 0; i < perceptron_list_size; i++)
    {
      perceptron_list[i]->reset();
    }
}

void PerceptronBP::btbUpdate(ThreadID tid, Addr branch_addr, void *&bp_history)
{
  // we don't need this function
}

bool PerceptronBP::lookup(ThreadID tid, Addr branch_addr, void *&bp_history)
{
  // find the index of the perceptron
  int perceptron_index = (branch_addr >> instShiftAmt) & (perceptron_list_size - 1);
  // grap the perceptron we need to calculate prediction
  Perceptron* new_perceptron = perceptron_list[perceptron_index];
  // generate elements needed for history struct
  int perceptron_output = new_perceptron->predict(global_history);
  // insert into history object
  BPHistory *history = new BPHistory;
  history->perceptron_output = perceptron_output;
  history->global_history = global_history;
  // update the referenced bp_history with the history we created
  bp_history = static_cast<void*>(history);
  // update our global history instance variable
  global_history.insert(global_history.begin() + 1, ((perceptron_output >= 0)? 1 : -1));
  global_history.pop_back();
        
  return perceptron_output >= 0;
}

void PerceptronBP::update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history, bool squashed)
{
  // we can only proceed if we have a history object
  if(bp_history != NULL)
    {
      //cout << "entered bp_hitory NO NULL\n";
      // cast the void pointer to our struct type
      BPHistory *history = static_cast<BPHistory*>(bp_history);
      int prev_branch_act = taken? 1 : -1;
      if(squashed)
        {
	  // if this path is being squashed we rever our global history
	  global_history = history->global_history;
	  global_history.insert(global_history.begin() + 1, prev_branch_act);
	  global_history.pop_back();
	  return;
        }
      // find the index of the perceptron
      int perceptron_index = (branch_addr >> instShiftAmt) & (perceptron_list_size - 1);
      // grab the perceptron we need to train
      Perceptron* new_perceptron = perceptron_list[perceptron_index];
      // train perceptron
      new_perceptron->train(min_confidence, history->global_history, history->perceptron_output, prev_branch_act);
      // delete unnecesary pointer
      delete history;
    }
}

void PerceptronBP::squash(ThreadID tid, void *bp_history)
{
  // in order to get he deconstructors to run, we need to cast our history
  // before removing it
  BPHistory *history = static_cast<BPHistory*>(bp_history);
  // recover global history
  global_history = history->global_history;
  // delete element
  delete history;
}

void PerceptronBP::uncondBranch(ThreadID tid, Addr pc, void *&bp_history)
{
  // An unconditional branch is just a taken branch
  BPHistory *history = new BPHistory;
  history->perceptron_output =  1; // assume taken output
  history->global_history = global_history;
  bp_history = static_cast<void *>(history);
  // update gobal history
  global_history.insert(global_history.begin() + 1, 1);
  global_history.pop_back();
}

PerceptronBP *PerceptronBPParams::create()
{
  return new PerceptronBP(this);
}
