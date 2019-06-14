#!/bin/bash
#
# Uses (start,finish) hard cycle values calculated from flat 100M simulation of
#   the SPEC 2017 benchmarks to fast-forward to a certain part of the exe's and
#   run from there to some fixed end-point.
#
# Author: Rach Liu

#source bench_stat.sh
source bench_spec.sh

# Truth dir
gdir="./golden/taggedpf-100M/"

# The tests to run
odir="$outdir/fastforward"
runtests="${tests[@]}"
#runtests=(perlbench gcc mcf omnetpp xalancbmk x264)

# Fast forwarding options
ff_opts="--standard-switch=0 --warmup-insts="


start_runs() {
  conf_opts_copy="$conf_opts" # copy the default settings before modification
  # c1 is # of cycles to 30%
  # c2 is # of cycles from 30% to 70%
  # actual c's below are cycles/3 as a rough approx to # of insts
  for t in ${runtests[@]}; do
    if [ "$t" = "perlbench" ]; then
      c1=26000000
      c2=34000000
    elif [ "$t" = "gcc" ]; then
      c1=24000000
      c2=33000000
    elif [ "$t" = "mcf" ]; then
      c1=73000000
      c2=95000000
    elif [ "$t" = "omnetpp" ]; then
      c1=06000000
      c2=85000000
    elif [ "$t" = "xalancbmk" ]; then
      c1=183000000
      c2=247000000
    elif [ "$t" = "x264" ]; then
      c1=22000000
      c2=073000000
    elif [ "$t" = "leela" ]; then
      c1=71000000
      c2=97000000
    elif [ "$t" = "exchange2" ]; then
      c1=87000000
      c2=11000000
    fi
    ml_opts="" # uncomment to turn off perceptron
    conf_opts="$ff_opts$c1 $conf_opts_copy"
#    echo -e "conf_opts: $conf_opts\n"
    run_one "$odir" "$t"
  done
}

print_stats() {
  local stat="$1"
  print_series "$odir" "$stat" "${runtests[@]}"
}

print_stats_2d() {
  for labl in ${runtests[@]}; do
    local gf="$gdir/$labl/stats.txt"
    if [ -f $gf ]; then
      print_line "$gdir/$labl/stats.txt" "$labl" $@
    fi
  done
  for labl in ${runtests[@]}; do
    print_line "$odir/$labl/stats.txt" "$labl" $@
  done
}

parse_args() {
  if [ "$1" = "run" ]; then
    start_runs
  else
    # print_stats $@
    print_stats_2d $@
  fi
}

parse_args $@


