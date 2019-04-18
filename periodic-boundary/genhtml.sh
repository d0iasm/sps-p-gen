#!/bin/bash

# Constant definition.
SRC=$(pwd)
DEST="../../sps-p-out/periodic-boundary/"
INDEX="index.html"
MAX_GEN=100000

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
  for i in {-8..12}; do
    for j in {-8..12}; do
      local p=`echo $i | awk '{ printf "%.1f", $1 / 10 }'`
      local m=`echo $j | awk '{ printf "%.1f", $1 / 10 }'`
      local file=$DEST"abpm_0.8,0.4,"$p","$m".html"
      ./generator -k2 0.8 0.4 $p $m -gen $MAX_GEN > $file 
      echo Generated $file
    done
  done
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

