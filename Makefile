SRC=src
PUBLIC=public

MAXGEN=200000
RANGE=-0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2

o: generator
	./src/generator-o -gen $(MAXGEN) -k2 0.8 0.4 0.4 0.4 > test-o.html
	./src/generator-o -gen $(MAXGEN) -k2 0.8 0.4 0.4 0.4 -dynamic > test-o-d.html

p: generator
	./src/generator-p -gen $(MAXGEN) -k2 0.8 0.4 0.4 0.4 > test-p.html

p20: generator
	./src/generator-p -gen $(MAXGEN) -k2 0.8 0.4 0.4 0.4 -cycle 20 > test-p-c20.html
	./src/generator-p -gen $(MAXGEN) -k2 0.8 0.4 0.4 0.4 -cycle 20 -dynamic > test-p-c20-d.html

p30: generator
	  ./src/generator-p -gen $(MAXGEN) -k2 0.8 0.4 0.4 0.4 -cycle 30 > test-p-c30.html
	  ./src/generator-p -gen $(MAXGEN) -k2 0.8 0.4 0.4 0.4 -cycle 30 -dynamic > test-p-c30-d.html

test: o p p20 p30

generator:
	make -C src generator

dynamic: generator
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic '>' 'abpm=0.8,0.4,{1},{2}\&b=open\&d=true.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&d=true.html' ::: $(RANGE) ::: $(RANGE)

html: generator
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) '>' 'abpm=0.8,0.4,{1},{2}\&b=open.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic '>' 'abpm=0.8,0.4,{1},{2}\&b=open\&d=true.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&d=true.html' ::: $(RANGE) ::: $(RANGE)

index:
	./gen_index.sh

public: html
	cp -a css $(PUBLIC)
	cp -a js $(PUBLIC)
	./gen_index.sh
	mv abpm* $(PUBLIC)

clean:
	make -C src clean
	rm *.html


# === REMOVE ? ===

all: html-sh csv csve

html-sh:
	./genhtml.sh -open
	./genhtml.sh -periodic
	./genhtml.sh -open -dist
	./genhtml.sh -periodic -dist

csv:
	./gencsv.sh -open
	./gencsv.sh -periodic

csve:
	./gencsve.sh -open
	./gencsve.sh -periodic
