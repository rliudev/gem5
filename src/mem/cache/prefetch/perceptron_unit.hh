/*
 * Author: Victor Fu
 *         Thomas Chang
 *         Rach Liu
 *         Daan Leiva
 */

#ifndef __MEM_CACHE_PREFETCH_PERCEPTRON_UNIT_HH__
#define __MEM_CACHE_PREFETCH_PERCEPTRON_UNIT_HH__

#include <vector>

#include "base/types.hh"
#include "sim/clocked_object.hh"
#include "sim/sim_object.hh"

#include "mem/cache/prefetch/perceptron.hh"



struct PerceptronUnitParams;

class PerceptronUnit : public ClockedObject
{
  protected:

  private:
  public:
    // contains pointers all of the perceptrons that will be use for predictions
    std::vector<Perceptron*> perceptron_list;
    // contains the global history results
    std::vector<int> global_history;
    // contains the per-perceptron last prediction
    std::vector<int> prediction_history;
    // raw size of our our perceptron list
    int perceptron_list_size;
    // size of the perceptrons that will be generated (number of inputs) (also size of X)
    int perceptron_size;
    // required confidence to accept a perceptron output
    int min_confidence;
    // timeout for each prefetch address, in # of cache accesses
    int pf_timeout;
    // reject all offered prefetch addresses?
    bool reject_all;
    // accept all offered prefetch addresses?
    bool accept_all;
    const int accept_table_size = 256;
    const int deny_table_size = 32;
    Addr last_pc = 0;  

    std::vector<std::vector<std::tuple<Addr, std::vector<double>>>> accept_table;
    std::vector<std::vector<std::tuple<Addr, std::vector<double>>>> deny_table;
    // std::vector<std::tuple<Addr, std::vector<std::vector<int>>>> deny_table;
    std::vector<std::tuple<Addr, std::vector<std::vector<int>>>> test_vec;

    // Queue whose max length indicates the last element (ie
    //   element that spent the longest time in queue) has
    //   timed out (ie, was not used for prefetching).
    std::vector<std::vector<Addr>> pf_timer_queue;


    // used as a history structure to match the general BPredUnit class
    // basically the inputs we need to hold to run the train method
    struct PFHistory
    {
      int perceptron_output;
      std::vector<int> global_history;
    };

  
  
    using AddrPriority = std::pair<Addr, int32_t>;  // Copied over from queued.hh

    /*
     * Contructor for the perceptron unit that accepts params.
     * @param exponential_size the value will generate a ~ 2^n size perceptron table
     * @param perceptron_size  size of perceptrons generated (number of inputs)
     */
    PerceptronUnit(const PerceptronUnitParams *p);

    /**
     * Shoudl we prefetch?
     */
    void shouldPrefetch(std::vector<AddrPriority> &addresses);


    /**
     * Self-update timed out (if any) pf entries.
     */
    void updateExpiredPfs();


    /**
     * Insert new pf addresses to the beginning of our timing queue.
     */
    void queuePfAddrs(std::vector<Addr> addrs);


    /**
     * On a cache hit to an address, we want to all references to
     *   that address, by replacing with empty (invalidate).
     *   Replacing with empty ensures timer queue maintains proper
     *   timing.
     */
    void invalidatePfAddrs(Addr hitAddr);

    /**
     * Does the pf timer queue have a timed-out pf entry?
     */
    bool hasTimedOutEntry() {
      return pf_timer_queue.size() >= pf_timeout;
    }


    /**
     * Looks up the given address in the prefetch predictor and returns
     * a true/false value as to whether it is taken
     * @param prefetch_addr The address of the prefetch to look up
     * @param pf_history pointer to the bp history
     * @return Whether or not the prefetch is takend
     */
    bool lookup(std::vector<double> features);

    /**
     * Updates the prefetch predictor with the actual result of a prefetch
     * @param prefetch_addr The address of the prefetch to update
     * @param used Whether or not the prefetch was used
     */
    void update(std::vector<double> features, bool used);

    /*
     * Squashes a path that was mispredicted
     * @param tid the id of the thread being executed
     * @param pf_history pointer to history structure
     */
    void squash(ThreadID tid, void *pf_history);

    /*
     * Resets all of the structures to their original state
     */
    void reset();

};

#endif //__MEM_CACHE_PREFETCH_PERCEPTRON_HH__



