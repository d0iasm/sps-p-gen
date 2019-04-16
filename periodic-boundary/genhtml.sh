#!/bin/bash

out="../../sps-p-out/periodic/"
index="index.html"
maxgen=100000

make generator

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

cd $out
echo "<h1>SPS-P Model Simulation</h1><ul>" > $index
for i in *.html; do echo "<li><a href="$i">$i</a></li>"; done >> $index
echo "</ul>" >> $index
echo Done Write to $index
