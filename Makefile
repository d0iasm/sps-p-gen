SRC=src
JSON=json
LOCAL=.
LOCAL_IMG=$(LOCAL)/img
# Development environment is only for tsukumo computer.
DEV=../public_html
DEV_IMG=$(PUBLIC_LOCAL)/img
PUBLIC=public
PUBLIC_IMG=$(PUBLIC)/img
UTIL=utils
UTIL_K=$(UTIL)/stackplot.py
UTIL_ENERGY=$(UTIL)/logplot.py

ENV=MPLBACKEND=Agg

MAXGEN=10000
# RANGE=-0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2
RANGE = -0.8 -0.6 -0.4 -0.2 0.0 0.2 0.4 0.6 0.8 1.0 1.2
# INITS = zero random
INITS = random
# SEEDS = 0 1 2 3 4
SEEDS = 0 
EXE=generator-o generator-p

generator:
	make -C src generator

html: generator
	parallel $(SRC)/generator-o -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(LOCAL)/sps-p\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.html' ::: $(INITS) ::: $(SEEDS)
	parallel $(SRC)/generator-p -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(LOCAL)/sps-p\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.html' ::: $(INITS) ::: $(SEEDS)

json:
	parallel $(SRC)/generator-o -json -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(JSON)/sps-p\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' ::: $(INITS) ::: $(SEEDS)
	parallel $(SRC)/generator-p -json -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(JSON)/sps-p\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' ::: $(INITS) ::: $(SEEDS)

img-static-energy: json
	parallel $(ENV) python3 utils/logplot.py -src '$(JSON)/sps-p\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(LOCAL_IMG)/static_energy\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 utils/logplot.py -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(LOCAL_IMG)/static_energy\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)

img-dynamic-energy: json
	parallel $(ENV) python3 utils/logplot.py -dynamic -src '$(JSON)/sps-p\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(LOCAL_IMG)/dynamic_energy\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(LOCAL_IMG)/dynamic_energy\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)

img: img-static-energy img-dynamic-energy

public: html img
	cp -a css $(PUBLIC)
	cp -a js $(PUBLIC)
	cp -a img $(PUBLIC)
	mv abpm* $(PUBLIC)
	mv index.html $(PUBLIC)

clean:
	make -C src clean
	rm *.html
