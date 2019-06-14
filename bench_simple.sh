#!/bin/bash
#
# Author: Rach Liu

source bench_spec.sh
source bench_stat.sh

# Truth dir
gdir="./golden/taggedpf-100M/"

# The tests to run
odir="$outdir"
runtests="${tests[@]}"
#runtests=(perlbench gcc mcf omnetpp xalancbmk x264)


start_runs() {
  run_series "$odir/simple" "${runtests[@]}"
}

print_stats() {
  local stat="$1"
  print_series "$odir/simple" "$stat" "${runtests[@]}"
}

print_stats_2d() {
  for labl in ${runtests[@]}; do
    local gf="$gdir/$labl/stats.txt"
    if [ -f $gf ]; then
      print_line "$gdir/$labl/stats.txt" "$labl" $@
    fi
  done
  for labl in ${runtests[@]}; do
    print_line "$odir/simple/$labl/stats.txt" "$labl" $@
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


