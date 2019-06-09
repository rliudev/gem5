#!/usr/bin/bash

source run.sh

run () {
  local x="$1"
  run-l2-tagged "$x"
}

name=lfsr-tagged-pbuf
xvar=(1 2 4 32 64 128 256)


for x in ${xvar[@]}; do
  echo "Run x: $x"
  run $x
done




