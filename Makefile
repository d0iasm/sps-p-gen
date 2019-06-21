test:
	make -C src generator
	./src/generator-o -gen 10000 -k2 0.8 0.4 0.4 0.4 > test-o.html
	./src/generator-p -gen 10000 -k2 0.8 0.4 0.4 0.4 > test-p.html

all: html csv csve

html:
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

clean:
	make -C src clean
	rm *.html
