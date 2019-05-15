#!/bin/bash
# Generate html files for open boundary.

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
  local range="-0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2"
  parallel ./generator -k2 '0.8 0.4 {1} {2}' -gen $MAX_GEN '>' $OUT_OB/'abpm=0.8,0.4,{1},{2}.html' ::: $range ::: $range
}

make_index() {
  cd $OUT_OB
  echo "<h1>SPS-P Model Simulation: Open Boundary</h1>\n<ul>" > $INDEX
  for i in *.html; do echo "<li><a href="$i">$i</a></li>"; done >> $INDEX
  echo "</ul>" >> $INDEX
  echo Generated $OUT_OB/$INDEX
  cd $ROOT/$OB
}

copy_js() {
  local dest=$OUT_OB/script.js
  if [ -f $dest ]; then
    rm $dest 
  fi
  cp ./script.js $dest 
}

copy_css() {
  local dest=$OUT_OB/style.css
  if [ -f $dest ]; then
    rm $dest 
  fi
  cp ./style.css $dest 
}

# Execution.
exepath=$(pwd)/$(diename $0)
source $exepath/../config.sh
check_dependencies 
make generator
exec_generator
make_index
copy_js
copy_css

