CLANG=clang++
CFLAGS=-O2 -ffast-math -march=native -std=c++11
INCLUDE=-I../include -I./

src += generator.cpp
include ../common/Makefile.inc

generator: $(files)
	$(clang) $(include) $(cflags) -o generator $(src)

clean:
	rm generator
