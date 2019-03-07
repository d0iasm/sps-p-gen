#!/bin/bash

out="out/"
maxgen=50000

clang++ -ffast-math -O3 -march=native -std=c++11 -o generator generator.cpp

for i in {-8..12}
do
  for j in {-8..12}
  do
    p=`echo $i | awk '{ printf "%.1f", $1 / 10 }'`
    m=`echo $j | awk '{ printf "%.1f", $1 / 10 }'`
    file=$out"abpm_0.8,0.4,"$p","$m".html"
    ./generator -k2 0.8 0.4 $p $m -gen $maxgen > $file 
    echo $file
  done
done

for i in *.html; do echo "<a href="$i">$i</a><br>"; done > index.html
