#!/usr/bin/bash

src=lfsr
cpu=derivO3
#cpu=atomic

itb=system.cpu.itb
dtb=system.cpu.dtb

sysvars=(final_tick sim_ticks system.cpu.Branches)
imemvars=($itb.rdAccesses $itb.wrAccesses)
dmemvars=($dtb.rdAccesses $dtb.wrAccesses)


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
  get-truth "${imemvars[@]}"
  get-truth "${dmemvars[@]}"

  echo -e "--------------------"
  echo -e "Test:\n"

  get-test "${sysvars[@]}"
  get-test "${imemvars[@]}"
  get-test "${dmemvars[@]}"
}



compare


