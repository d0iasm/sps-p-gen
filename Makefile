CLANG=clang++
CFLAGS=-O0 -march=native -std=c++11
CFLAGS-FAST=-ffast-math -O3 -march=native -std=c++11
FILES=generator.cpp energy.cpp xv.cpp

generator:
	$(CLANG) $(CFLAGS) -o generator $(FILES)

generator-fast:
	$(CLANG) $(CFLAGS-FAST) -o generator $(FILES)
