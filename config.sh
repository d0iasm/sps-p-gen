#!/bin/bash

echo ====== File path configuration ======

# Variables for experiment.
MAX_GEN=200

# Variables for each directory.
OB=open-boundary
OBD=open-boundary-dist
PB=periodic-boundary
PBD=periodic-boundary-dist

CSV=csv
IMG=images

# Project root directory.
exepath=$(pwd)/$(dirname $0)
curpath=$(pwd)
cd $exepath/..
ROOT=$(pwd)
cd $curpath
echo root directory: $ROOT

# Data directories. 
DATA_CSV=$ROOT/$CSV
DATA_IMG=$ROOT/$IMG
echo csv data directory: $DATA_CSV
echo image directory: $DATA_IMG

# Output directory for static web hosting publicly.
OUT=$ROOT/public
echo output directory: $OUT

# Output directory corresponding to each directory.
OUT_OB=$OUT/$OB
OUT_OBD=$OUT/$OBD
OUT_PB=$OUT/$PB
OUT_PBD=$OUT/$PBD
echo output directory for open boundary: $OUT_OB
echo output directory for open boundary dist: $OUT_OBD
echo output directory for periodic boundary: $OUT_PB
echo output directory for periodic boundary dist: $OUT_PBD
echo =====================================

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

