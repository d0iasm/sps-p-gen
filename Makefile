CLANG=clang++
CFLAGS=-ffast-math -O3 -march=native -std=c++11
CFLAGS-RELEASE=-O0 -std=c++11
FILES=generator.cpp energy.cpp

generator:
	$(CLANG) $(CFLAGS) -o generator $(FILES)

generator-release:
	$(CLANG) $(CFLAGS-RELEASE) -o generator $(FILES)
