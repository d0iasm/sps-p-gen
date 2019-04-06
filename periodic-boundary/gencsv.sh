#!/bin/bash

maxgen=100

make generator

file="csv/result.csv"
echo "k1,k00,k01,k10,k11,k2,ka,kb,kp,km,HBEaverage,HBEvariancen" > $file

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
