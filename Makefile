SRC=src
JSON=json
IMG=img
PUBLIC=public
PUBLIC_IMG=public/img
PUBLIC_LOCAL=../public_html
PUBLIC_LOCAL_IMG=../public_html/img

ENV=MPLBACKEND=Agg

MAXGEN=10000
# RANGE=-0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2
RANGE = -0.8 -0.6 -0.4 -0.2 0.0 0.2 0.4 0.6 0.8 1.0 1.2
INIT = zero random
# SEEDS = 0 1 2 3 4
SEEDS = 0 
EXE=generator-o generator-p

generator:
	make -C src generator

html: generator
	parallel $(SRC)/generator-o -init zero -gen $(MAXGEN) -seed '{1}' '>' 'sps-p\&b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k=zero\&s={1}.html' ::: $(SEEDS) 
	parallel $(SRC)/generator-p -init zero -gen $(MAXGEN) -seed '{1}' '>' 'sps-p\&b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k=zero\&s={1}.html' ::: $(SEEDS) 

hoge:
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) '>' 'abpm=0.8,0.4,{1},{2}\&b=open.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic global '>' 'abpm=0.8,0.4,{1},{2}\&b=open\&d=global.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic local '>' 'abpm=0.8,0.4,{1},{2}\&b=open\&d=local.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic.html' ::: $(RANGE) ::: $(RANGE)
	# parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -cycle 20 '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&c=20.html' ::: $(RANGE) ::: $(RANGE)
	# parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -cycle 40 '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&c=40.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic global '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&d=global.html' ::: $(RANGE) ::: $(RANGE)
	# parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic global -cycle 20 '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&d=global\&c=20.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic local '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&d=local.html' ::: $(RANGE) ::: $(RANGE)
	# parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic local -cycle 20 '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&d=local\&c=20.html' ::: $(RANGE) ::: $(RANGE)
	./gen_index.sh

public: html img
	cp -a css $(PUBLIC)
	cp -a js $(PUBLIC)
	cp -a img $(PUBLIC)
	mv abpm* $(PUBLIC)
	mv index.html $(PUBLIC)

clean:
	make -C src clean
	rm *.html
