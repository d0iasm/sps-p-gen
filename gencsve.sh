#!/bin/bash
# Generate csv data of open/periodic boundary and images for each parameter.
# Usage:
#    gencsve.sh [-open/-periodic]

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


exec_generator_csve() {
  local dir=$DATA_CSV/$out
  if [ ! -d $dir ]; then
    mkdir $dir
  fi

  parallel ./generator -k2 0.8 0.4 '{1}' '{2}' -gen $MAX_GEN -csve '>' $dir/'0.8,0.4,{1},{2}.csv' ::: $RANGE ::: $RANGE
}

generate_images() {
  local tool=$UTIL/logplot.py
  local src=$DATA_CSV/$out

  parallel python3 $tool -k '0.8 0.4 {1} {2}' -src $src/'0.8,0.4,{1},{2}.csv' ::: $RANGE ::: $RANGE
}

mv_images() {
  local dir=$DATA_IMG/$out
  if [ ! -d $dir ]; then
    mkdir $dir
  fi

  mv *.png $dir
}

# Execution.
exepath=$(pwd)/$(dirname $0)
source ./config.sh
check_dependencies

# Set flags for open or periodic.
target=$OB
out=open-boundary-energy
if [ "$1" = "-periodic" ]; then
  echo "Generate csv files and images for periodic boundary."
  target=$PB
  out=periodic-boundary-energy
fi

if [ "$1" = "-open" ]; then
  echo "Generate csv files and images for open boundary."
  target=$OB
  out=open-boundary-energy
fi

cd $ROOT/$target
make generator
exec_generator_csve
generate_images
mv_images
cd $exepath

