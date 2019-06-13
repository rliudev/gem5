#!/bin/bash

outdir="~/output/run1"
mkdir -p $outdir

pf=TaggedPrefetcher
cpu=DerivO3CPU

cmd=./build/X86/gem5.opt
config=configs/example/se.py

build=victor_jun10-m64
basedir=run/run_base_refrate_"$build".0000/
tests=(perlbench gcc mcf omnetpp xalancbmk x264 leela exchange2)




run_bench() {
  local name="$1"
  local exe="$2"
  local opts="$3"

  arg="--caches  --l2cache --cmd=$exe  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=$cpu  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --maxinsts=10000000"

  $cmd $config $arg --l2-hwp-type=$pf --mlu_pf_type=perceptron
}



run_gcc() {
  run_bench "gcc" "cpugcc_r_base.$build" '--options="ref32.c -O3 -fselective-scheduling -fselective-scheduling2 -o ref32.opts-O3_-fselective-scheduling_-fselective-scheduling2.s"'
}


run() {
  local t=$1
  if echo ${tests[@]} | grep -q -w "$t"; then
    echo -e "Running SPEC test: $t\n"
  else
    echo -e "Error: SPEC test: $t is not valid.\n"
    exit 1
  fi

  go $t
  cd $basedir
  run_$1()
  cp -a m5out/. $outdir/$t
}


