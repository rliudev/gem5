#include "mem/cache/prefetch/perceptron.hh"

#include <cassert>

#include "base/intmath.hh"
#include "base/logging.hh"
#include "base/random.hh"
#include "base/trace.hh"
#include "debug/HWPrefetch.hh"
#include "mem/cache/replacement_policies/base.hh"
#include "params/PerceptronPrefetcher.hh"

// PerceptronPrefetcher::StrideEntry::StrideEntry()
// {
//     invalidate();
// }

// void
// PerceptronPrefetcher::StrideEntry::invalidate()
// {
//     instAddr = 0;
//     lastAddr = 0;
//     isSecure = false;
//     stride = 0;
//     confidence = 0;
// }

    // : QueuedPrefetcher(p),
    //   maxConf(p->max_conf),
    //   threshConf(p->thresh_conf),
    //   minConf(p->min_conf),
    //   startConf(p->start_conf),
    //   pcTableAssoc(p->table_assoc),
    //   pcTableSets(p->table_sets),
    //   useMasterId(p->use_master_id),
    //   degree(p->degree),
    //   replacementPolicy(p->replacement_policy)

PerceptronPrefetcher::PerceptronPrefetcher(const PerceptronPrefetcherParams *p)
            : QueuedPrefetcher(p)
{
    // assert(isPowerOf2(pcTableSets));
}

// PerceptronPrefetcher::PCTable*
// PerceptronPrefetcher::findTable(int context)
// {
//     // Check if table for given context exists
//     auto it = pcTables.find(context);
//     if (it != pcTables.end())
//         return &it->second;

//     // If table does not exist yet, create one
//     return allocateNewContext(context);
// }

// PerceptronPrefetcher::PCTable*
// PerceptronPrefetcher::allocateNewContext(int context)
// {
//     // Create new table
//     auto insertion_result = pcTables.insert(std::make_pair(context,
//         PCTable(pcTableAssoc, pcTableSets, name(), replacementPolicy)));

//     DPRINTF(HWPrefetch, "Adding context %i with stride entries\n", context);

//// Get iterator to new pc table, and then return a pointer to the new table
//     return &(insertion_result.first->second);
// }

//PerceptronPrefetcher::PCTable::PCTable(int assoc, int sets,
// const std::string name,
//                                    BaseReplacementPolicy* replacementPolicy)
//     : pcTableSets(sets), _name(name), entries(pcTableSets),
//       replacementPolicy(replacementPolicy)
// {
//     for (int set = 0; set < sets; set++) {
//         entries[set].resize(assoc);
//         for (int way = 0; way < assoc; way++) {
//             // Inform the entry its position
//             entries[set][way].setPosition(set, way);

//             // Initialize replacement policy data
//             entries[set][way].replacementData =
//                 replacementPolicy->instantiateEntry();
//         }
//     }
// }

// PerceptronPrefetcher::PCTable::~PCTable()
// {
// }

void
PerceptronPrefetcher::calculatePrefetch(const PrefetchInfo &pfi,
                                    std::vector<AddrPriority> &addresses)
{
    // if (!pfi.hasPC()) {
    //     DPRINTF(HWPrefetch, "Ignoring request with no PC.\n");
    //     return;
    // }

    // // Get required packet info
    // Addr pf_addr = pfi.getAddr();
    // Addr pc = pfi.getPC();
    // bool is_secure = pfi.isSecure();
    // MasterID master_id = useMasterId ? pfi.getMasterId() : 0;

    // // Get corresponding pc table
    // PCTable* pcTable = findTable(master_id);

    // // Search for entry in the pc table
    // StrideEntry *entry = pcTable->findEntry(pc, is_secure);

    // if (entry != nullptr) {
    //     // Hit in table
    //     int new_stride = pf_addr - entry->lastAddr;
    //     bool stride_match = (new_stride == entry->stride);

    //     // Adjust confidence for stride entry
    //     if (stride_match && new_stride != 0) {
    //         if (entry->confidence < maxConf)
    //             entry->confidence++;
    //     } else {
    //         if (entry->confidence > minConf)
    //             entry->confidence--;
    //// If confidence has dropped below the threshold, train new stride
    //         if (entry->confidence < threshConf)
    //             entry->stride = new_stride;
    //     }

    //     DPRINTF(HWPrefetch, "Hit: PC %x pkt_addr %x (%s) stride %d (%s), "
    //             "conf %d\n", pc, pf_addr, is_secure ? "s" : "ns",
    //             new_stride, stride_match ? "match" : "change",
    //             entry->confidence);

    //     entry->lastAddr = pf_addr;

    //     // Abort prefetch generation if below confidence threshold
    //     if (entry->confidence < threshConf)
    //         return;

    //     // Generate up to degree prefetches
    //     for (int d = 1; d <= degree; d++) {
    //         // Round strides up to atleast 1 cacheline
    //         int prefetch_stride = new_stride;
    //         if (abs(new_stride) < blkSize) {
    //             prefetch_stride = (new_stride < 0) ? -blkSize : blkSize;
    //         }

    //         Addr new_addr = pf_addr + d * prefetch_stride;
    //         addresses.push_back(AddrPriority(new_addr, 0));
    //     }
    // } else {
    //     // Miss in table
    //     DPRINTF(HWPrefetch, "Miss: PC %x pkt_addr %x (%s)\n", pc, pf_addr,
    //             is_secure ? "s" : "ns");

    //     StrideEntry* entry = pcTable->findVictim(pc);

    //     // Invalidate victim
    //     entry->invalidate();
    //     replacementPolicy->invalidate(entry->replacementData);

    //     // Insert new entry's data
    //     entry->instAddr = pc;
    //     entry->lastAddr = pf_addr;
    //     entry->isSecure = is_secure;
    //     entry->confidence = startConf;
    //     replacementPolicy->reset(entry->replacementData);
    // }
}

// inline Addr
// PerceptronPrefetcher::PCTable::pcHash(Addr pc) const
// {
//     Addr hash1 = pc >> 1;
//     Addr hash2 = hash1 >> floorLog2(pcTableSets);
//     return (hash1 ^ hash2) & (Addr)(pcTableSets - 1);
// }

// inline PerceptronPrefetcher::StrideEntry*
// PerceptronPrefetcher::PCTable::findVictim(Addr pc)
// {
//     // Rand replacement for now
//     int set = pcHash(pc);

//     // Get possible entries to be victimized
//     std::vector<ReplaceableEntry*> possible_entries;
//     for (auto& entry : entries[set]) {
//         possible_entries.push_back(&entry);
//     }

//     // Choose victim based on replacement policy
//     StrideEntry* victim = static_cast<StrideEntry*>(
//         replacementPolicy->getVictim(possible_entries));

//     DPRINTF(HWPrefetch, "Victimizing lookup table[%d][%d].\n",
//             victim->getSet(), victim->getWay());

//     return victim;
// }

// inline PerceptronPrefetcher::StrideEntry*
// PerceptronPrefetcher::PCTable::findEntry(Addr pc, bool is_secure)
// {
//     int set = pcHash(pc);
//     for (auto& entry : entries[set]) {
//         // Search ways for match
//         if ((entry.instAddr == pc) && (entry.isSecure == is_secure)) {
//  DPRINTF(HWPrefetch, "Lookup hit table[%d][%d].\n", entry.getSet(),
//                     entry.getWay());
//             replacementPolicy->touch(entry.replacementData);
//             return &entry;
//         }
//     }
//     return nullptr;
// }

PerceptronPrefetcher*
PerceptronPrefetcherParams::create()
{
    return new PerceptronPrefetcher(this);
}

