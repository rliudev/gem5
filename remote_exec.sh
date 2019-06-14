#!/bin/bash
# Get remote results
# Author: Rach LIu


remoteuser=victorfu
remotepw=finalproject

rundir='~/rliu_run/'


remote_exec() {
  rsync -az --delete configs $remoteuser@tetracosa:$rundir
  rsync -az --delete build/X86/gem5.opt $remoteuser@tetracosa:$rundir
}


remote_exec


