#!/bin/bash

source bench_spec.sh


# The tests to run
runtests="${tests[@]}"
#runtests=(perlbench gcc mcf omnetpp xalancbmk x264)


run_series "$outdir/simple" "${runtests[@]}"


