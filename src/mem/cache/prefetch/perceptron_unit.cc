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
  perceptron_size = perceptron_sizes[curMode];
  min_confidence = perceptron_size*2+14;

  // in order to use lower bits as look up values we need to make sure the resulting
  // value is a power of 2
  if(!isPowerOf2(perceptron_list_size))
  {
    fatal("Invalid perceptron list size!\n");
  }

  printf("PerceptronUnit is in mode: %s\n", getModeStr().c_str());


  if (curMode == PAST_PREDICTIONS) {
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


  else if (curMode == PC_DELTA_ADDR) {
    perceptron = new Perceptron(perceptron_size);
    last_pc = 0;
  }


}

void PerceptronUnit::shouldPrefetch(const PrefetchInfo &pfi, std::vector<AddrPriority> &addresses)
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
    bool shouldUse = lookup(pfi, it->first);
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
    std::pair<const PrefetchInfo*, std::vector<Addr>> p = pf_timer_queue.back();
    const PrefetchInfo *pfi = p.first;
    std::vector<Addr> timed_out_list = p.second;
    pf_timer_queue.pop_back();
    for (auto exp_addr : timed_out_list) {
      update(pfi, exp_addr, false);
    }
  }
}


void PerceptronUnit::queuePfAddrs(const PrefetchInfo *pfi, const std::vector<Addr> addrs) {
  pf_timer_queue.insert(
    pf_timer_queue.begin(),
    std::pair<const PrefetchInfo*, std::vector<Addr>>(pfi, addrs)
  );
}


void PerceptronUnit::invalidatePfAddrs(Addr hitAddr) {
  for (auto it = pf_timer_queue.begin(); it != pf_timer_queue.end(); it++) {
    std::vector<Addr> pf_addrs = it->second;
    for (auto it2 = pf_addrs.begin(); it2 != pf_addrs.end(); ) {
      if ( *it2 == hitAddr) {
        it2 = pf_addrs.erase(it2);
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


bool PerceptronUnit::lookup(const PrefetchInfo &pfi, Addr pf_addr)
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


  if (curMode == PAST_PREDICTIONS) {
    int perceptron_index = (pf_addr >> INST_SHIFT_AMT) & (perceptron_list_size - 1);
    Perceptron* perceptron = perceptron_list[perceptron_index];
    int perceptron_output = perceptron->predict(global_history);

    // update our global history instance variable
    global_history.insert(global_history.begin() + 1, ((perceptron_output >= 0)? 1 : -1));
    global_history.pop_back();

    // store into prediction_history
    prev_pfh[&pfi] = new PFHistory(pf_addr, perceptron_output, &global_history);

    return perceptron_output >= 0;
  }


  else if (curMode == PC_DELTA_ADDR) {
    Addr delta = pfi.getPC() - pf_addr;
    std::vector<int> *x = new std::vector<int>();
    x->push_back(1);           // bias
    x->push_back(pfi.getAddr() - last_pc); // feature 1:  pc
    x->push_back(delta);       // feature 2:  delta-addr
    int perceptron_output = perceptron->predict(*x);

    last_pc = pfi.getPC();
//    if (prev_pfh.find(&pfi) == prev_pfh.end()) {
//      panic("This shouldn't happen. PFI's are supposed to be unique.\n");
//    }
    prev_pfh[&pfi] = new PFHistory(pf_addr, perceptron_output, x);

    return perceptron_output >= 0;
  }


  return 0;
}


void PerceptronUnit::update(const PrefetchInfo *pfi, Addr pf_addr, bool used)
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

  if (curMode == PAST_PREDICTIONS) {
    const PFHistory *pfh = prev_pfh[pfi];
    int actual_pf_act = used? 1 : -1;
    // find the index of the perceptron
    int perceptron_index = (pf_addr >> INST_SHIFT_AMT) & (perceptron_list_size - 1);
    // grab the perceptron we need to train
    Perceptron* new_perceptron = perceptron_list[perceptron_index];
    // train perceptron
    new_perceptron->train(min_confidence, *(pfh->xn), pfh->p_out, actual_pf_act);
  }

  else if (curMode == PC_DELTA_ADDR) {
    // Update them all
    const PFHistory *pfh = prev_pfh[pfi];
    perceptron->train(min_confidence, *(pfh->xn), pfh->p_out, used? 1 : -1);
  }

}


PerceptronUnit *PerceptronUnitParams::create()
{
  return new PerceptronUnit(this);
}


