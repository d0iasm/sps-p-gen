SRC=src
JSON=json
IMG=img
PUBLIC=public

ENV=MPLBACKEND=Agg

MAXGEN=200
# RANGE=-0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2
RANGE = -0.8 -0.4 0.0 0.4 0.8 1.2

generator:
	make -C src generator

json: generator
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=open\&d=true.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&d=true.json' ::: $(RANGE) ::: $(RANGE)

img: json
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=true.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=true.json' ::: $(RANGE) ::: $(RANGE)

html: generator
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) '>' 'abpm=0.8,0.4,{1},{2}\&b=open.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic '>' 'abpm=0.8,0.4,{1},{2}\&b=open\&d=true.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&d=true.html' ::: $(RANGE) ::: $(RANGE)
	./gen_index.sh

public: html
	cp -a css $(PUBLIC)
	cp -a js $(PUBLIC)
	mv abpm* $(PUBLIC)
	mv index.html $(PUBLIC)

clean:
	make -C src clean
	rm *.html

# === Will remove the following functions. ===
csv:
	./gencsv.sh -open
	./gencsv.sh -periodic

csve:
	./gencsve.sh -open
	./gencsve.sh -periodic
