SRC=src
PUBLIC=public

MAXGEN=200000
RANGE=-0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2

test:
	make -C src generator
	./src/generator-o -gen 100000 -k2 0.8 0.4 0.4 0.4 > test-o.html
	./src/generator-p -gen 100000 -k2 0.8 0.4 0.4 0.4 > test-p.html
	./src/generator-p -gen 100000 -k2 0.8 0.4 0.4 0.4 -cycle 20 > test-p-c20.html
	./src/generator-p -gen 100000 -k2 0.8 0.4 0.4 0.4 -cycle 30 > test-p-c30.html

generator:
	make -C src generator

html: generator
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) '>' 'abpm=0.8,0.4,{1},{2}&b=open.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) '>' 'abpm=0.8,0.4,{1},{2}&b=periodic.html' ::: $(RANGE) ::: $(RANGE)

public: html
	cp -a css $(PUBLIC)
	cp -a js $(PUBLIC)
	echo "<h1>SPS-P</h1><ul>" > index.html
	for i in $(shell ls *.html) ; do \
	  echo "<li><a href="$${i}">$${i}</a></li>" >> index.html ; \
	done
	echo "</ul>" >> index.html
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
