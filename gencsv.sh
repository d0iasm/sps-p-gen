#!/bin/bash

maxgen=50000

clang++ -ffast-math -O3 -march=native -std=c++11 -o generator generator.cpp

file="csv/result.csv"
echo "k1,k00,k01,k10,k11,k2,ka,kb,kp,km,HBEaverage,HBEvariance\n" > $file

for i in {-8..12}
do
  for j in {-8..12}
  do
    p=`echo $i | awk '{ printf "%.1f", $1 / 10 }'`
    m=`echo $j | awk '{ printf "%.1f", $1 / 10 }'`
    ./generator -k2 0.8 0.4 $p $m -gen $maxgen -csv >> $file 
    echo "done 0.8 0.4 "$p $m 
  done
done