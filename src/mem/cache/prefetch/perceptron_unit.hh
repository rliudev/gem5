/*
 * Author: Victor Fu
 *         Thomas Chang
 *         Rach Liu
 *         Daan Leiva
 */

#ifndef __MEM_CACHE_PREFETCH_PERCEPTRON_UNIT_HH__
#define __MEM_CACHE_PREFETCH_PERCEPTRON_UNIT_HH__

#include <vector>
#include <unordered_map>

#include "base/types.hh"
#include "sim/clocked_object.hh"
#include "sim/sim_object.hh"

#include "mem/cache/prefetch/prefetch_info.hh"
#include "mem/cache/prefetch/perceptron.hh"



struct PerceptronUnitParams;

class PerceptronUnit : public ClockedObject
{
  public:

    // Enable different features:
    enum FeatureSet {
      PAST_PREDICTIONS = 0,
      PC_DELTA_ADDR
    };
    FeatureSet curMode = PAST_PREDICTIONS;
    //FeatureSet curMode = PC_DELTA_ADDR;

    // used as a history structure; needed to hold the previous inputs
    //   so that we can run the train method (if needed).
    class PFHistory
    {
      public:
        Addr pf_addr;              // the pf_addr that generated this pfh
        int p_out;                 // perceptron_output for this pf
        std::vector<int> *xn;      // input features for this pf

        PFHistory(Addr pf_addr, int p_out, std::vector<int> *xn)
          : pf_addr(pf_addr), p_out(p_out), xn(xn)
        {
        }
    };

  private:
    int perceptron_list_size;    // raw size of our our perceptron list
    int perceptron_size = -1;    // unused
    int min_confidence;          // required confidence to accept a perceptron output
    int pf_timeout;              // timeout for each prefetch address, in # of cache accesses
    bool reject_all;             // reject all offered prefetch addresses?
    bool accept_all;             // accept all offered prefetch addresses?

    // Queue whose max length indicates the last element (ie
    //   element that spent the longest time in queue) has
    //   timed out (ie, was not used for prefetching).
    std::vector<std::pair<const PrefetchInfo*, std::vector<Addr>>> pf_timer_queue;

    // size of the perceptrons that will be generated (number of inputs) (also size of X)
    // array dim is the size for the chosen FeatureSet.
    int perceptron_sizes[2] = {
      20,   // PAST_PREDICTIONS
      3     // PC_DELTA_ADDR
    };

    // Feature sets:
    //
    // 0. Y/N history (prediction history over time)
    //    - multiple perceptrons
    //    - contains pointers all of the perceptrons that will be use for predictions
    //    - features:  <prediction[0], prediction[1], ... prediction[19]> where index is temporal (result for past cache miss)
    std::vector<Perceptron*> perceptron_list;   // multiple perceptrons
    std::vector<int> global_history;            // contains the global history results
    std::vector<int> prediction_history;        // contains the per-perceptron last prediction
    //
    // 1. PC & DeltaAddr Tracking
    //    - single perceptron
    //    - features: <last cache miss's pc, current pc, delta addr: current_pc - predicted_addr>
    Perceptron *perceptron;                        // a single perceptron
    Addr last_pc;                                  // the pc of the last cache miss.
    std::unordered_map<Addr, int> deltas;          // track the outcome per delta-addr
    std::unordered_map<const PrefetchInfo*, PFHistory*> prev_pfh; // store prev prefetches for soem prefetch addr


  public:
    using AddrPriority = std::pair<Addr, int32_t>;  // Copied over from queued.hh

    /*
     * Contructor for the perceptron unit that accepts params.
     * @param exponential_size the value will generate a ~ 2^n size perceptron table
     * @param perceptron_size  size of perceptrons generated (number of inputs)
     */
    PerceptronUnit(const PerceptronUnitParams *p);

    std::string getModeStr() {
      if (curMode == PAST_PREDICTIONS) {
        return "PAST_PREDICTIONS";
      }
      else if (curMode == PC_DELTA_ADDR) {
        return "PC_DELTA_ADDR";
      }
      return "UHHH-WAT";
    }

    /**
     * Shoudl we prefetch?
     */
    void shouldPrefetch(const PrefetchInfo &pfi, std::vector<AddrPriority> &addresses);


    /**
     * Self-update timed out (if any) pf entries.
     */
    void updateExpiredPfs();


    /**
     * Insert new pf addresses to the beginning of our timing queue.
     */
    void queuePfAddrs(const PrefetchInfo *pfi, std::vector<Addr> addrs);


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
    bool lookup(const PrefetchInfo &pfi, Addr prefetch_addr);

    /**
     * Updates the prefetch predictor with the actual result of a prefetch
     * @param prefetch_addr The address of the prefetch to update
     * @param used Whether or not the prefetch was used
     */
    void update(const PrefetchInfo *pfi, Addr prefetch_addr, bool used);

    /*
     * Resets all of the structures to their original state
     */
    void reset();

};

#endif //__MEM_CACHE_PREFETCH_PERCEPTRON_UNIT_HH__



