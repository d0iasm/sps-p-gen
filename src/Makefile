CLANG=clang++
CFLAGS=-O2 -ffast-math -march=native -std=c++11

TOP=$(CURDIR)
INCLUDE=-I$(TOP)/include

common-files = generator.cpp energy.cpp xv.cpp dynamic.cpp

src-o += $(common-files)
include $(TOP)/open-boundary/Makefile.inc

src-p += $(common-files)
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
