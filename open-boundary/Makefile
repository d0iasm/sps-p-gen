CLANG=clang++
CFLAGS=-O2 -ffast-math -march=native -std=c++11
FILES=generator.cpp energy.cpp xv.cpp

generator: $(FILES)
	$(CLANG) $(CFLAGS) -o generator $(FILES)

clean:
	rm generator
