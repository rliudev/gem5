/*
 * Author: Victor Fu
 *         Thomas Chang
 *         Rach Liu
 *         Daan Leiva
 */

#ifndef __MEM_CACHE_PREFETCH_PERCEPTRON_HH__
#define __MEM_CACHE_PREFETCH_PERCEPTRON_HH__

#include <string>
#include <unordered_map>
#include <vector>

#include "base/types.hh"
#include "mem/packet.hh"


struct PerceptronPfParams;

class PerceptronPf
{
  protected:

  private:
    // contains pointers all of the perceptrons that will be use for predictions
    std::vector<Perceptron*> perceptron_list;
    // contains the global history results
    std::vector<int> global_history;
    // raw size of our our perceptron list
    int perceptron_list_size;
    // size of the perceptrons that will be generated (number of inputs) (also size of X)
    int perceptron_size;
    // required confidence to accept a perceptron output
    int min_confidence;
    // used as a history structure to match the general BPredUnit class
    // basically the inputs we need to hold to run the train method
    struct BPHistory
    {
      int perceptron_output;
      std::vector<int> global_history;
    };

  public:
    PerceptronPf(const PerceptronPfParams *p);

    bool shouldPrefetch(const PrefetchInfo &pfi,
                        std::vector<AddrPriority> &addresses)
    {

    }

    /*
     * Contructor for the perceptron handler
     * @param exponential_size the value will generate a ~ 2^n size perceptron table
     * @param perceptron_size  size of perceptrons generated (number of inputs)
     * @param min_confidence minimum confidence needed to validate an output
     */
    PerceptronPf(const PerceptronPfParams *params);

    /*
     * Function during an unconditional branch. Treated as a taken branch.
     * @param tid the id of the thread being executed
     * @param pc instruction address
     * @param bp_history pointer to the bp history
     */
    void uncondBranch(ThreadID tid, Addr pc, void * &bp_history);

    /**
     * Looks up the given address in the branch predictor and returns
     * a true/false value as to whether it is taken
     * @param branch_addr The address of the branch to look up
     * @param bp_history pointer to the bp history
     * @return Whether or not the branch is takend
     */
    bool lookup(ThreadID tid, Addr branch_addr, void *&bp_history);

    /**
     * Updates the branch predictor to Not Taken if a BTB entry is
     * invalid or not found.
     * @param tid the id of the thread being executed
     * @param branch_addr The address of the branch to look up
     * @param bp_history pointer to the bp history
     * @return Whether or not the branch is taken
     */
    void btbUpdate(ThreadID tid, Addr branch_addr, void *&bp_history);

    /**
     * Updates the branch predictor with the actual result of a branch
     * @param tid the id of the thread being executed
     * @param branch_addr The address of the branch to update
     * @param taken Whether or not the branch was taken
     * @param bp_history pointer to the bp history
     * @param squashed tells us if the history has been deleted
     */
    void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history, bool squashed);

    /*
     * Squashes a path that was mispredicted
     * @param tid the id of the thread being executed
     * @param bp_history pointer to history structure
     */
    void squash(ThreadID tid, void *bp_history);

    /*
     * Resets all of the structures to their original state
     */
    void reset();

};

#endif //__MEM_CACHE_PREFETCH_PERCEPTRON_HH__



