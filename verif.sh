#!/usr/bin/bash

src=lfsr
cpu=derivO3
#cpu=atomic

itb=system.cpu.itb
dtb=system.cpu.dtb

sysvars=(final_tick sim_ticks system.cpu.Branches system.cpu.ipc)
memvars=(
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
)


get-lines () {
  local file="$1" && shift
  local vars=($@)
  for w in ${vars[@]}; do
    cat $file | grep "$w"
  done
}

get-truth () {
  get-lines "259tests/m5out-$src-$cpu/stats.txt" $@
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



compare


