CLANG=clang++
CFLAGS=-O2 -ffast-math -march=native -std=c++11

TOP=$(CURDIR)
INCLUDE=-I$(TOP)/include

src-o += generator.cpp
src-o += energy.cpp
src-o += xv.cpp
include $(TOP)/open-boundary/Makefile.inc

src-p += generator.cpp
src-p += energy.cpp
src-p += xv.cpp
include $(TOP)/periodic-boundary/Makefile.inc

generator:
	make open
	make periodic

open:
	$(CLANG) $(INCLUDE) $(CFLAGS) -o generator-o $(src-o)

periodic:
	$(CLANG) $(INCLUDE) $(CFLAGS) -o generator-p $(src-p)

clean:
	rm generator-*
