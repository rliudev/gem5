#!/bin/bash
#
# Author:  Rach Liu
#

source bash_utils.sh
source bench_stat.sh


GEM5HOME="$HOME/finalproject/gem5"
SPECHOME="$HOME/cpu2017"

outdir="$HOME/output/bench_spec"

pf=TaggedPrefetcher
cpu=DerivO3CPU

cmd=$GEM5HOME/build/X86/gem5.opt
#cmd=echo
conf=$GEM5HOME/configs/example/se.py

# opts defaults
#    Just override these if need to 'parametrize' over them in some outter loop
#    Set them every time before a call
m5_opts=""
conf_opts="--caches  --l2cache --l1i_size=16kB  --l1d_size=64kB --l2_size=256kB --cpu-type=$cpu  --mem-type=DDR3_1600_8x8  --cpu-clock=1GHz --maxinsts=10000000"
sim_opts=""
pf_opts="--l2-hwp-type=$pf"
ml_opts="--mlu_pf_type=perceptron"


build=victor_jun10-m64
basedir=run/run_base_refrate_"$build".0000/
tests=(perlbench gcc mcf omnetpp xalancbmk x264 leela exchange2)


mkdir -p $outdir
cd $SPECHOME && source shrc



run_bench() {
  local name="$1" && shift
  local exe="$1"  && shift
  bash -c "$cmd $m5_opts $conf --cmd=$exe $conf_opts $sim_opts $pf_opts $ml_opts"
}


run_perlbench() {
  sim_opts="--options=\"-I./lib checkspam.pl 2500 5 25 11 150 1 1 1 1\""
  run_bench "perlbench" "perlbench_r_base.$build"
}

run_gcc() {
  sim__opts="--options=\"ref32.c -O3 -fselective-scheduling -fselective-scheduling2 -o ref32.opts-O3_-fselective-scheduling_-fselective-scheduling2.s\""
  run_bench "gcc" "cpugcc_r_base.$build"
}

run_mcf() {
  sim_opts="--options='inp.in'"
  run_bench "mcf" "mcf_r_base.$build"
}

run_omnetpp() {
  sim_opts="--options=\"-c General -r\""
  run_bench "omnetpp" "omnetpp_r_base.$build"
}

run_xalancbmk() {
  sim_opts="--options=\"-v t5.xml xalanc.xsl\""
  run_bench "xalancbmk" "cpuxalan_r_base.$build"
}

run_x264() {
  sim_opts="--options=\"--pass 2 --stats x264_stats.log --bitrate 1000 --dumpyuv 200 --frames 1000 -o BuckBunny_New.264 BuckBunny.yuv 1280x720\""
  run_bench "x264" "x264_r_base.$build"
}

run_leela() {
  sim_opts="--options=\"ref.sgf\""
  run_bench "leela" "leela_r_base.$build"
}

run_exchange2() {
  sim_opts="--options=\"6\""
  run_bench "exchange2" "exchange2_r_base.$build"
}


run() {
  local t=$1 && shift
  if echo ${tests[@]} | grep -q -w "$t"; then
    echo -e "Running SPEC test: $t\n"
  else
    echo -e "Error: SPEC test: $t is not valid.\n"
    exit 1
  fi

  go $t
  cd $basedir


  if [ "$t" = "perlbench" ]; then
    run_perlbench
  elif [ "$t" = "gcc" ]; then
    run_gcc
  elif [ "$t" = "mcf" ]; then
    run_mcf
  elif [ "$t" = "omnetpp" ]; then
    run_omnetpp
  elif [ "$t" = "xalancbmk" ]; then
    run_xalancbmk
  elif [ "$t" = "x264" ]; then
    run_x264
  elif [ "$t" = "leela" ]; then
    run_leela
  elif [ "$t" = "exchange2" ]; then
    run_exchange2
  fi

}


# Sample usage:
#    mkdir -p "$odir/$t"
#    cp_m5out "$odir/$t"
cp_m5out() {
  local dst="$1"
  cp -a m5out/. "$1"
}


run_series() {
  local odir="$1" && shift
  local arr=($@)
  for t in ${arr[@]}; do
    max_bg_procs 15
    mkdir -p "$odir/$t"
    m5_opts="--outdir=\"$odir/$t\""
    run $t > "$odir/$t/run.log" 2>&1 &
  done
}


# Use if you need to loop over each run call yourself
run_one() {
  local odir="$1" && shift
  local t="$1" && shift
  max_bg_procs 15
  mkdir -p "$odir/$t"
  m5_opts="--outdir=\"$odir/$t\""
  run $t > "$odir/$t/run.log" 2>&1 &
}


# $1: input dir
# $2: metric you want
# $3: [array] subrun dirs
print_series() {
  local odir="$1" && shift
  local stat="$1" && shift
  local arr=($@)
  for t in ${arr[@]}; do
    print_line "$odir/$t/stats.txt" "$t" "$stat"
  done
}


