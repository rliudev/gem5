#!/bin/bash
#
# Author: Rach Liu

#source bench_stat.sh
source bench_spec.sh

# Truth dir
gdir="./golden/taggedpf-100M/"

# The tests to run
odir="$outdir/simple"
runtests="${tests[@]}"
#runtests=(perlbench gcc mcf omnetpp xalancbmk x264)

# View dir for stats
vdir="./bench_results/1_ff--perceptron1"

start_runs() {
  run_series "$odir" "${runtests[@]}"
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
    print_line "$vdir/$labl/stats.txt" "$labl" $@
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


