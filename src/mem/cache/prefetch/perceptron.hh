#ifndef __MEM_CACHE_PREFETCH_PERCEPTRON_HH__
#define __MEM_CACHE_PREFETCH_PERCEPTRON_HH__

#include <string>
#include <unordered_map>
#include <vector>

#include "base/types.hh"
#include "mem/cache/prefetch/queued.hh"
#include "mem/cache/replacement_policies/replaceable_entry.hh"
#include "mem/packet.hh"

class BaseReplacementPolicy;
struct PerceptronPrefetcherParams;

class PerceptronPrefetcher : public QueuedPrefetcher
{
  protected:
    // const int maxConf;
    // const int threshConf;
    // const int minConf;
    // const int startConf;

    // const int pcTableAssoc;
    // const int pcTableSets;

    // const bool useMasterId;

    // const int degree;

    // /** Replacement policy used in the PC tables. */
    // BaseReplacementPolicy* replacementPolicy;

    // struct StrideEntry : public ReplaceableEntry
    // {
    //     /** Default constructor */
    //     StrideEntry();

    //     /** Invalidate the entry */
    //     void invalidate();

    //     Addr instAddr;
    //     Addr lastAddr;
    //     bool isSecure;
    //     int stride;
    //     int confidence;
    // };

    // class PCTable
    // {
    //   public:
    //     /**
    //      * Default constructor. Create a table with given parameters.
    //      *
    //      * @param assoc Associativity of the table.
    //      * @param sets Number of sets in the table.
    //      * @param name Name of the prefetcher.
    //      * @param replacementPolicy Replacement policy used by the table.
    //      */
    //     PCTable(int assoc, int sets, const std::string name,
    //             BaseReplacementPolicy* replacementPolicy);

    //     /**
    //      * Default destructor.
    //      */
    //     ~PCTable();

    //     /**
    //      * Search for an entry in the pc table.
    //      *
    //      * @param pc The PC to look for.
    //      * @param is_secure True if the target memory space is secure.
    //      * @return Pointer to the entry.
    //      */
    //     StrideEntry* findEntry(Addr pc, bool is_secure);

    //     /**
    //      * Find a replacement victim to make room for given PC.
    //      *
    //      * @param pc The PC value.
    //      * @return The victimized entry.
    //      */
    //     StrideEntry* findVictim(Addr pc);

      private:
    //     const std::string name() {return _name; }
    //     const int pcTableSets;
    //     const std::string _name;
    //     std::vector<std::vector<StrideEntry>> entries;

    //     /**
    //      * Replacement policy used by StridePrefetcher.
    //      */
    //     BaseReplacementPolicy* replacementPolicy;

    //     /**
    //      * PC hashing function to index sets in the table.
    //      *
    //      * @param pc The PC value.
    //      * @return The set to which this PC maps.
    //      */
    //     Addr pcHash(Addr pc) const;
    // };
    // std::unordered_map<int, PCTable> pcTables;

    // /**
    //  * Try to find a table of entries for the given context. If none is
    //  * found, a new table is created.
    //  *
    //  * @param context The context to be searched for.
    //  * @return The table corresponding to the given context.
    //  */
    // PCTable* findTable(int context);

    // /**
    //  * Create a PC table for the given context.
    //  *
    //  * @param context The context of the new PC table.
    //  * @return The new PC table
    //  */
    // PCTable* allocateNewContext(int context);

  public:
    PerceptronPrefetcher(const PerceptronPrefetcherParams *p);

    void calculatePrefetch(const PrefetchInfo &pfi,
                           std::vector<AddrPriority> &addresses) override;
};

#endif //__MEM_CACHE_PREFETCH_PERCEPTRON_HH__
