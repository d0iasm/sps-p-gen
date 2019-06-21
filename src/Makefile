CLANG=clang++
CFLAGS=-O2 -ffast-math -march=native -std=c++11
INCLUDE=-I./include

src += generator.cpp
src += energy.cpp
src += xv.cpp

open:
	include ./open-boundary/Makefile.inc
	make generator

periodic:
	include ./periodic-boundary/Makefile.inc
	make generator

generator: $(files)
	$(clang) $(include) $(cflags) -o generator $(src)

clean:
	rm generator
