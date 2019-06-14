#!/bin/bash

source bench_spec.sh


# The tests to run
runtests="${tests[@]}"


run_series "$outdir/simple" "${runtests[@]}"


