#!/bin/bash
# Generate html files for periodic boundary.

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
  if [ ! -d $OUT_PB ]; then
    mkdir $OUT_PB
  fi

  parallel ./generator -k2 '0.8 0.4 {1} {2}' -gen $MAX_GEN '>' $OUT_PB/'abpm=0.8,0.4,{1},{2}.html' ::: $RANGE ::: $RANGE
}

make_index() {
  cd $OUT_PB
  local dest=$OUT_PB/index.html
  echo "<h1>SPS-P Model Simulation: Periodic Boundary</h1><ul>" > $dest
  for i in *.html; do echo "<li><a href="$i">$i</a></li>"; done >> $dest
  echo "</ul>" >> $dest
  echo Generate $dest
  cd $ROOT/$PB
}

copy_js() {
  local dest=$OUT_PB/script.js
  if [ -f $dest ]; then
    rm $dest 
  fi
  echo Copy from $ROOT/$PB/script.js to $dest
  cp ./script.js $dest 
}

copy_css() {
  local dest=$OUT_PB/style.css
  if [ -f $dest ]; then
    rm $dest 
  fi
  echo Copy from $ROOT/$PB/style.css to $dest
  cp ./style.css $dest 
}

# Execution.
exepath=$(pwd)/$(dirname $0)
source $exepath/../config.sh
check_dependencies 

cd $ROOT/$PB
make generator
exec_generator
make_index
copy_js
copy_css
cd $exepath
