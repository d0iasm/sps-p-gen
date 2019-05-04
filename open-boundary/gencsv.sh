#!/bin/bash

MAX_GEN=100
OUT="csv/result.csv"

# Execution.
make generator
echo "k1,k00,k01,k10,k11,k2,ka,kb,kp,km,class,evergy(average),energy(variance)" > $OUT

for i in {-8..12}
do
  for j in {-8..12}
  do
    p=`echo $i | awk '{ printf "%.1f", $1 / 10 }'`
    m=`echo $j | awk '{ printf "%.1f", $1 / 10 }'`
    ./generator -k2 0.8 0.4 $p $m -gen $MAX_GEN -csv >> $OUT 
    echo "done 0.8 0.4 "$p $m 
  done
done
