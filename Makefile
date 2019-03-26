CLANG=clang++
CFLAGS=-O0 -march=native -std=c++11
CFLAGS-FAST=-ffast-math -O3 -march=native -std=c++11
FILES=generator.cpp energy.cpp xv.cpp

generator: $(FILES)
	$(CLANG) $(CFLAGS) -o generator $(FILES)

generator-fast: $(FILES)
	$(CLANG) $(CFLAGS-FAST) -o generator $(FILES)
