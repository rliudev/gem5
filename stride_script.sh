#!/usr/bin/bash

pf=TaggedPrefetcher

go perlbench
cd run/run_base_refrate_victor_jun10-m64.0000/

/home/victorfu/finalproject/gem5/build/X86/gem5.opt /home/victorfu/finalproject/gem5/configs/example/se.py --caches  --l2cache --cmd=perlbench_r_base.victor_jun10-m64  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=DerivO3CPU  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --options="-I./lib checkspam.pl 2500 5 25 11 150 1 1 1 1" --maxinsts=10000000 --l2-hwp-type=$pf --mlu_pf_type=perceptron

cp -a m5out/. /home/victorfu/output/PERLBENCH/

go gcc
cd run/run_base_refrate_victor_jun10-m64.0000/

/home/victorfu/finalproject/gem5/build/X86/gem5.opt /home/victorfu/finalproject/gem5/configs/example/se.py  --caches  --l2cache  --cmd=cpugcc_r_base.victor_jun10-m64  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=DerivO3CPU  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --options="ref32.c -O3 -fselective-scheduling -fselective-scheduling2 -o ref32.opts-O3_-fselective-scheduling_-fselective-scheduling2.s" --maxinsts=10000000 --l2-hwp-type=$pf --mlu_pf_type=perceptron

cp -a m5out/. /home/victorfu/output/GCC/

go mcf
cd run/run_base_refrate_victor_jun10-m64.0000/

/home/victorfu/finalproject/gem5/build/X86/gem5.opt /home/victorfu/finalproject/gem5/configs/example/se.py  --caches  --l2cache  --cmd=mcf_r_base.victor_jun10-m64  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=DerivO3CPU  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --options='inp.in' --maxinsts=10000000 --l2-hwp-type=$pf --mlu_pf_type=perceptron

cp -a m5out/. /home/victorfu/output/MCF/

go omnetpp
cd run/run_base_refrate_victor_jun10-m64.0000/

/home/victorfu/finalproject/gem5/build/X86/gem5.opt /home/victorfu/finalproject/gem5/configs/example/se.py  --caches  --l2cache  --cmd=omnetpp_r_base.victor_jun10-m64  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=DerivO3CPU  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --options="-c General -r" --maxinsts=10000000 --l2-hwp-type=$pf --mlu_pf_type=perceptron

cp -a m5out/. /home/victorfu/output/OMNETPP/

go xalancbmk
cd run/run_base_refrate_victor_jun10-m64.0000/

/home/victorfu/finalproject/gem5/build/X86/gem5.opt /home/victorfu/finalproject/gem5/configs/example/se.py  --caches  --l2cache  --cmd=cpuxalan_r_base.victor_jun10-m64  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=DerivO3CPU  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --options="-v t5.xml xalanc.xsl" --maxinsts=10000000 --l2-hwp-type=$pf --mlu_pf_type=perceptron

cp -a m5out/. /home/victorfu/output/XALANCBMK/

go x264
cd run/run_base_refrate_victor_jun10-m64.0000/

/home/victorfu/finalproject/gem5/build/X86/gem5.opt /home/victorfu/finalproject/gem5/configs/example/se.py  --caches  --l2cache  --cmd=x264_r_base.victor_jun10-m64  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=DerivO3CPU  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --options="--pass 2 --stats x264_stats.log --bitrate 1000 --dumpyuv 200 --frames 1000 -o BuckBunny_New.264 BuckBunny.yuv 1280x720" --maxinsts=10000000 --l2-hwp-type=$pf --mlu_pf_type=perceptron

cp -a m5out/. /home/victorfu/output/x264/

go leela
cd run/run_base_refrate_victor_jun10-m64.0000/

/home/victorfu/finalproject/gem5/build/X86/gem5.opt /home/victorfu/finalproject/gem5/configs/example/se.py  --caches  --l2cache  --cmd=leela_r_base.victor_jun10-m64  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=DerivO3CPU  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz  --options="ref.sgf" --maxinsts=10000000 --l2-hwp-type=$pf --mlu_pf_type=perceptron

cp -a m5out/. /home/victorfu/output/LEELA/

go exchange2
cd run/run_base_refrate_victor_jun10-m64.0000/

/home/victorfu/finalproject/gem5/build/X86/gem5.opt /home/victorfu/finalproject/gem5/configs/example/se.py  --caches  --l2cache  --cmd=exchange2_r_base.victor_jun10-m64  --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=DerivO3CPU  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --options="6" --maxinsts=10000000 --l2-hwp-type=$pf --mlu_pf_type=perceptron

cp -a m5out/. /home/victorfu/output/EXCHANGE2/





