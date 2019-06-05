#!/usr/bin/bash

./build/X86/gem5.opt configs/example/se.py --cmd=259tests/lfsr.out --caches  --l2cache --cmd=259tests/lfsr.out  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=DerivO3CPU  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --maxinsts=10000000 --l2-hwp-type=StridePrefetcher
