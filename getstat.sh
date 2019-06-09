#!/usr/bin/bash

# truth is the path to the file that you want to compare against, ie
#     truth="259tests/m5out-lfsr-stride-derivO3/stats.txt"
# vars are the metrics in stats.sh that you want to list.
#     Make sure each var only matches 1 metric
# pfqlen is the x-axis
#
truth="$1"
vars=("${@:2}")
pfqlen=(1 2 4 32 64 128 256)


# enable if need debug
#echo -e "truth: $truth\n"
#echo -e "vars: ${vars[*]}\n"


get-lines () {
  local file="$1"
  local var="$2"
  _ret=$(grep "$var" $file | awk '{print $2}')
}


# Print header row
arr=()
for v in ${vars[@]}; do
  arr+=("$v\t")
done
echo -e "len\t${arr[*]}"

# Print results for truth benchmark
arr=()
for v in ${vars[@]}; do
  get-lines "$truth" "$v"
  arr+=("$_ret\t")
done
echo -e "-bench-\t${arr[*]}"

# Print results for data
for len in ${pfqlen[@]}; do
  arr=()
  for v in ${vars[@]}; do
    get-lines "data/lfsr-tagged-pbuf$len.txt" "$v"
    arr+=("$_ret\t")
  done
  echo -e "$len\t${arr[*]}"
done


