#!/bin/bash
# Get remote results
# Author: Rach LIu


remoteuser=victorfu
remotepw=finalproject

rundir='~/rliu_run/'

cmd="./bench_fastforward"

remote_exec() {
  rsync -az --delete *.sh $remoteuser@tetracosa:$rundir
  rsync -az --delete configs $remoteuser@tetracosa:$rundir
  rsync -az --delete build/X86/gem5.opt $remoteuser@tetracosa:$rundir
  su $remoteuser && cd $HOME/$rundir && $cmd
}


remote_exec


