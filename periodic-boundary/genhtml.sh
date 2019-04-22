#!/bin/bash
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


# Constant definition.
SRC=$(pwd)
DEST="../../sps-p-out/periodic-boundary/"
INDEX="index.html"
MAX_GEN=100

# Function definition.
check_dependencies() {
  if ! [ -x "$(command -v clang++)" ]; then
    echo 'Error: clang++ is not installed.' >&2
    exit 1
  fi
  if ! [ -x "$(command -v parallel)" ]; then
    echo 'Error: parallel is not installed.' >&2
    exit 1
  fi
}

exec_generator() {
  local range="-0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2"
  parallel ./generator -k2 0.8 0.4 '{1}' '{2}' -gen $MAX_GEN '>' $DEST'abpm=0.8,0.4,{1},{2}.html' ::: $range ::: $range
}

make_index() {
  cd $DEST
  echo "<h1>SPS-P Model Simulation</h1><ul>" > $INDEX
  for i in *.html; do echo "<li><a href="$i">$i</a></li>"; done >> $INDEX
  echo "</ul>" >> $INDEX
  echo Generated $INDEX
  cd $SRC
}

copy_js() {
  local dest=$DEST/script.js
  if [ -f $dest ]; then
    rm $dest 
  fi
  cp ./script.js $dest 
}

copy_css() {
  local dest=$DEST/style.css
  if [ -f $dest ]; then
    rm $dest 
  fi
  cp ./style.css $dest 
}

# Execution.
check_dependencies 
make generator
exec_generator
make_index
copy_js
copy_css

