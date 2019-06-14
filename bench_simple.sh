#!/bin/bash
#
# Author: Rach Liu

source bench_spec.sh
source bench_stat.sh


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


parse_args $@


