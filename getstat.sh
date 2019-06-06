#!/usr/bin/bash


metric=$1
file1=$2
file2=$3


get-lines () {
  local file="$1"
  local var="$2"
  cat $file | grep "$var"
}


get-lines "$file1" "$metric"
get-lines "$file2" "$metric"


