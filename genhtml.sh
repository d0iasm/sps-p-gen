#!/bin/bash
# Generate html files.
# Usage:
#   genhtml.sh [-open/-periodic] [-dist (option)]

# This shell script is using GNU Parallel.
# Because of an academic tradition, Cite:
# @book{tange_ole_2018_1146014,
#   author       = {Tange, Ole},
#   title        = {GNU Parallel 2018},
#   publisher    = {Ole Tange},
#   month        = Mar,
#   year         = 2018,
#   ISBN         = {9781387509881},
#   doi          = {10.5281/zenodo.1146014},
#   url          = {https://doi.org/10.5281/zenodo.1146014}
# }

# More about funding GNU Parallel and the citation notice:
# https://lists.gnu.org/archive/html/parallel/2013-11/msg00006.html
# https://www.gnu.org/software/parallel/parallel_design.html#Citation-notice
# https://git.savannah.gnu.org/cgit/parallel.git/tree/doc/citation-notice-faq.txt

# If you send a copy of your published article to tange@gnu.org, it will be
# mentioned in the release notes of next version of GNU Parallel.


exec_generator() {
  if [ ! -d $out ]; then
    mkdir $out
  fi

  parallel ./generator -k2 '0.8 0.4 {1} {2}' -gen $MAX_GEN $flag '>' $out/'abpm=0.8,0.4,{1},{2}.html' ::: $RANGE ::: $RANGE
}

make_index() {
  cd $out
  local dest=$out/index.html
  echo "<h1>$title</h1><ul>" > $dest
  for i in *.html; do echo "<li><a href="$i">$i</a></li>"; done >> $dest
  echo "</ul>" >> $dest
  echo Generate $dest
  cd $ROOT/$target
}

copy_js() {
  local dest=$out/script.js
  if [ -f $dest ]; then
    rm $dest 
  fi
  echo Copy from $ROOT/$target/script.js to $dest
  cp ./script.js $dest 
}

copy_css() {
  local dest=$out/style.css
  if [ -f $dest ]; then
    rm $dest 
  fi
  echo Copy from $ROOT/$target/style.css to $dest
  cp ./style.css $dest 
}

# Execution.
exepath=$(pwd)/$(dirname $0)
source ./config.sh
check_dependencies 

# Set flags for open/periodic.
target=$OB
out=$OUT_OB
title="SPS-P Model Simulation: Open Boundary"
if [ "$1" = "-periodic" ]; then
  echo "Generate html files for PERIODIC BOUNDARY."
  target=$PB
  out=$OUT_PB
  title="SPS-P Model Simulation: Periodic Boundary"
else
  echo "Generate html files for OPEN BOUNDARY."
fi

flag=""
if [ "$2" = "-dist" ]; then
  echo DIST flag is set.
  flag="-dist"
  if [ "$1" = "-periodic" ]; then
    out=$OUT_PBD
    title="SPS-P Model Simulation: Periodic Boundary Changing Based on Distance"
  else
    out=$OUT_OBD
    title="SPS-P Model Simulation: Open Boundary Changing Based on Distance"
  fi
fi

cd $ROOT/$target
make generator
exec_generator
make_index
copy_js
copy_css
cd $exepath
