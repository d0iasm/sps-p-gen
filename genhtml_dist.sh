#!/bin/bash

out="out/"
index="out/index.html"
maxgen=100000

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

cd out/
echo "<h1>SPS-P Model Simulation</h1><ul>" > index.html 
for i in *.html; do echo "<li><a href="$i">$i</a></li>"; done >> index.html
echo "</ul>" >> index.html 
echo Done Write to index.html 
