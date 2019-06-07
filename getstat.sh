#!/usr/bin/bash


metric=$1

get-lines () {
  local file="$1"
  local var="$2"
  cat $file | grep "$var"
}

pfqlen=(1 2 4 32 64 128 256)

for len in ${pfqlen[@]}; do
  echo "len: $len"
  get-lines "data/lfsr-tagged-pbuf$len.txt" "$metric"
done


