test:
	make -C open-boundary generator
	./open-boundary/generator -gen 10000 -k2 0.8 0.4 0.4 0.4 > test.html

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
