#!/usr/bin/bash

exe=259tests/lfsr.out
cpu=DerivO3CPU

cmd=./build/X86/gem5.opt
arg="configs/example/se.py --cmd=$exe --caches  --l2cache --cmd=$exe  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=$cpu  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --maxinsts=10000000"

# use StridePrefetcher for l2
run-l2-stride () {
  pf=StridePrefetcher
  $cmd $arg --l2-hwp-type=$pf --mlu_pf_type=perceptron
}

# use TaggeddPrefetcher for l2
run-l2-tagged () {
  pf=TaggedPrefetcher
  $cmd $arg --l2-hwp-type=$pf --mlu_pf_type=perceptron

}

# use no prefetcher (nullpf which is the defaulted pf)
run-no-pf () {
  $cmd $arg --mlu_pf_type=perceptron
}


#run-no-pf
#run-l2-tagged
run-l2-stride


