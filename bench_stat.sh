#!/usr/bin/bash
#
# Author: Rach Liu


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


print_header() {
  local xvar="$1" && shift
  local vars=($@)
  # Print header row
  arr=()
  for v in ${vars[@]}; do
    arr+=("$v\t")
  done
  echo -e "$xvar\t${arr[*]}"
}

# Print results for given file
print_line() {
  local file="$1" && shift
  local labl="$1" && shift
  local vars=($@)
  arr=()
  for v in ${vars[@]}; do
    get-lines "$file" "$v"
    arr+=("$_ret\t")
  done
  echo -e "$labl\t${arr[*]}"
}
