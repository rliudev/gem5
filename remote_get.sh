#!/bin/bash
# Get remote results
# Author: Rach LIu


remoteuser=victorfu
remotepw=finalproject
remotedir='~/output/bench_spec'


remote_get() {
  local dst=~/output
  mkdir -p $dst
  rsync -az --delete $remoteuser@tetracosa:$remotedir $dst
}



remote_get


