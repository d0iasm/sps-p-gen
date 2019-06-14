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
