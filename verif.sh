#!/usr/bin/bash

# format:  m5out-$src-$pf-$cpu
src=lfsr
#pf=stride
pf=tagged
#pf=null
cpu=derivO3
#cpu=atomic

itb=system.cpu.itb
dtb=system.cpu.dtb

sysvars=(final_tick sim_ticks system.cpu.Branches system.cpu.ipc)
memvars=(
##hwpf direct stats
system.l2.prefetcher.num_hwpf_issued
system.l2.prefetcher.pfBufferHit
system.l2.prefetcher.pfIdentified
system.l2.prefetcher.pfInCache
system.l2.prefetcher.pfRemovedFull
system.l2.prefetcher.pfSpanPage
system.l2.unused_prefetches
##demand accesses
system.l2.demand_misses::.cpu.inst
system.l2.demand_misses::.cpu.data
system.l2.demand_misses::total
system.l2.demand_hits::.cpu.inst
system.l2.demand_hits::.cpu.data
system.l2.demand_hits::total
##number of overall (read+write) accesses
system.l2.overall_accesses::.cpu.inst
system.l2.overall_accesses::.cpu.data
system.l2.overall_accesses::total
system.l2.overall_hits::.cpu.inst
system.l2.overall_hits::.cpu.data
system.l2.overall_hits::total
system.l2.overall_miss_latency::.cpu.inst
system.l2.overall_miss_latency::.cpu.data
system.l2.overall_miss_latency::total
system.l2.overall_miss_rate::.cpu.inst
system.l2.overall_miss_rate::.cpu.data
system.l2.overall_miss_rate::total
system.l2.overall_misses::.cpu.inst
system.l2.overall_misses::.cpu.data
system.l2.overall_misses::total
##average HardPFReq mshr miss latency/cycles/accesses/misses
system.l2.HardPFReq_avg_mshr_miss_latency::.l2.prefetcher
system.l2.HardPFReq_avg_mshr_miss_latency::total
system.l2.HardPFReq_mshr_miss_latency::.l2.prefetcher
system.l2.HardPFReq_mshr_miss_latency::total
system.l2.HardPFReq_mshr_miss_rate::.l2.prefetcher
system.l2.HardPFReq_mshr_miss_rate::total
system.l2.HardPFReq_mshr_misses::.l2.prefetcher
system.l2.HardPFReq_mshr_misses::total
##average qos
system.mem_ctrls.avgPriority_.cpu.inst::samples
system.mem_ctrls.avgPriority_.cpu.data::samples
system.mem_ctrls.avgPriority_.l2.prefetcher::samples
##read mem bw
system.mem_ctrls.bw_inst_read::.cpu.inst
system.mem_ctrls.bw_inst_read::total
system.mem_ctrls.bw_read::.cpu.inst
system.mem_ctrls.bw_read::.cpu.data
system.mem_ctrls.bw_read::.l2.prefetcher
system.mem_ctrls.bw_read::total
##total mem bw
system.mem_ctrls.bw_total::.writebacks
system.mem_ctrls.bw_total::.cpu.inst
system.mem_ctrls.bw_total::.cpu.data
system.mem_ctrls.bw_total::.l2.prefetcher
##number of bytes read
system.mem_ctrls.bytesReadDRAM
system.mem_ctrls.bytesReadSys
system.mem_ctrls.bytesReadWrQ
system.mem_ctrls.bytesWritten
system.mem_ctrls.bytesWrittenSys
system.mem_ctrls.bytes_inst_read::.cpu.inst
system.mem_ctrls.bytes_inst_read::total
system.mem_ctrls.bytes_read::.cpu.inst
system.mem_ctrls.bytes_read::.cpu.data
system.mem_ctrls.bytes_read::.l2.prefetcher
##per-master read serviced memory accesses
system.mem_ctrls.masterReadAccesses::.cpu.inst
system.mem_ctrls.masterReadAccesses::.cpu.data
system.mem_ctrls.masterReadAccesses::.l2.prefetcher
##per-master read average memory access latency
system.mem_ctrls.masterReadAvgLat::.cpu.inst
system.mem_ctrls.masterReadAvgLat::.cpu.data
system.mem_ctrls.masterReadAvgLat::.l2.prefetcher
##per-master bytes read from memory
system.mem_ctrls.masterReadBytes::.cpu.inst
system.mem_ctrls.masterReadBytes::.cpu.data
system.mem_ctrls.masterReadBytes::.l2.prefetcher
##per-master bytes read from memory (Bytes/sec)
system.mem_ctrls.masterReadRate::.cpu.inst
system.mem_ctrls.masterReadRate::.cpu.data
system.mem_ctrls.masterReadRate::.l2.prefetcher
##per-master read total memory access latency
system.mem_ctrls.masterReadTotalLat::.cpu.inst
system.mem_ctrls.masterReadTotalLat::.cpu.data
system.mem_ctrls.masterReadTotalLat::.l2.prefetcher
##number of read requests responded to by this memory
system.mem_ctrls.num_reads::.cpu.inst
system.mem_ctrls.num_reads::.cpu.data
system.mem_ctrls.num_reads::.l2.prefetcher
system.mem_ctrls.num_reads::total
system.mem_ctrls.num_writes::.writebacks
system.mem_ctrls.num_writes::total
system.mem_ctrls.pageHitRate
)


get-lines () {
  local file="$1" && shift
  local vars=($@)
  for w in ${vars[@]}; do
    cat $file | grep "$w"
  done
}

get-truth () {
  get-lines "259tests/m5out-$src-$pf-$cpu/stats.txt" $@
}

get-test () {
  get-lines "m5out/stats.txt" $@
}

compare () {
  echo -e "--------------------"
  echo -e "Ground truth:\n"

  get-truth "${sysvars[@]}"
  get-truth "${memvars[@]}"

  echo -e "\n\n"
  echo -e "--------------------"
  echo -e "Test:\n"

  get-test "${sysvars[@]}"
  get-test "${memvars[@]}"
}

diff-stats () {
  echo -e "\n\n\n"
  echo -e "--------------------"
  echo -e "Computing diff:\n"
  get-truth "${sysvars[@]}" >> _truth_.out
  get-truth "${memvars[@]}" >> _truth_.out
  get-test "${sysvars[@]}" >> _test_.out
  get-test "${memvars[@]}" >> _test_.out

  diff _truth_.out _test_.out

  rm -f _truth_.out
  rm -f _test_.out
}


compare
diff-stats


