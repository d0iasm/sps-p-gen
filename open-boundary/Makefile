CLANG=clang++
CFLAGS=-O2 -ffast-math -march=native -std=c++11
INCLUDE=../include
FILES=generator.cpp energy.cpp xv.cpp

generator: $(FILES)
	$(CLANG) -I$(INCLUDE) $(CFLAGS) -o generator $(FILES)

clean:
	rm generator
