SRC=src
JSON=json
LOCAL=.
LOCAL_IMG=$(LOCAL)/img
# Development environment is only for tsukumo computer.
DEV=../public_html
DEV_IMG=$(DEV)/img
PUBLIC=public
PUBLIC_IMG=$(PUBLIC)/img
UTIL=utils
UTIL_K=$(UTIL)/stackplot.py
UTIL_ENERGY=$(UTIL)/logplot.py
UTIL_XV=$(UTIL)/loglogplot.py

ENV=MPLBACKEND=Agg

MAXGEN=200000
# RANGE=-0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2
RANGE = -0.8 -0.6 -0.4 -0.2 0.0 0.2 0.4 0.6 0.8 1.0 1.2
INITS = zero random
# INITS = zero
# SEEDS = 0 1 2 3 4
SEEDS = 0
EXE=generator-o generator-p

generator:
	make -C src generator

html: generator
	parallel $(SRC)/generator-o -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(DEV)/sps-p\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.html' ::: $(INITS) ::: $(SEEDS)
	parallel $(SRC)/generator-o -dynamic static -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(DEV)/sps-p\?b=open\&c=-1\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.html' ::: $(INITS) ::: $(SEEDS)
	parallel $(SRC)/generator-o -dynamic dynamic -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(DEV)/sps-p\?b=open\&c=-1\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.html' ::: $(INITS) ::: $(SEEDS)

html-p: generator
	parallel $(SRC)/generator-p -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(DEV)/sps-p\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.html' ::: $(INITS) ::: $(SEEDS)
	parallel $(SRC)/generator-p -dynamic static -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(DEV)/sps-p\?b=periodic\&c=10\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.html' ::: $(INITS) ::: $(SEEDS)
	parallel $(SRC)/generator-p -dynamic dynamic -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(DEV)/sps-p\?b=periodic\&c=10\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.html' ::: $(INITS) ::: $(SEEDS)

json: generator
	parallel $(SRC)/generator-o -json -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(JSON)/sps-p\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' ::: $(INITS) ::: $(SEEDS)
	parallel $(SRC)/generator-o -json -dynamic static -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(JSON)/sps-p\?b=open\&c=-1\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.json' ::: $(INITS) ::: $(SEEDS)
	parallel $(SRC)/generator-o -json -dynamic dynamic -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(JSON)/sps-p\?b=open\&c=-1\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.json' ::: $(INITS) ::: $(SEEDS)

json-p: generator
	parallel $(SRC)/generator-p -json -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(JSON)/sps-p\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' ::: $(INITS) ::: $(SEEDS)
	parallel $(SRC)/generator-p -json -dynamic static -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(JSON)/sps-p\?b=periodic\&c=10\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.json' ::: $(INITS) ::: $(SEEDS)
	parallel $(SRC)/generator-p -json -dynamic dynamic -init '{1}' -gen $(MAXGEN) -seed '{2}' '>' \
	  '$(JSON)/sps-p\?b=periodic\&c=10\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.json' ::: $(INITS) ::: $(SEEDS)

img-energy: json
	parallel $(ENV) python3 $(UTIL_ENERGY) -src '$(JSON)/sps-p\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/energy\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_ENERGY) -src '$(JSON)/sps-p\?b=open\&c=-1\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/energy\?b=open\&c=-1\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_ENERGY) -src '$(JSON)/sps-p\?b=open\&c=-1\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/energy\?b=open\&c=-1\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)

img-energy-p: json-p
	parallel $(ENV) python3 $(UTIL_ENERGY) -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/energy\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_ENERGY) -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/energy\?b=periodic\&c=10\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_ENERGY) -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/energy\?b=periodic\&c=10\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)

img-kparam: json
	parallel $(ENV) python3 $(UTIL_K) -src '$(JSON)/sps-p\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/kparam\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_K) -src '$(JSON)/sps-p\?b=open\&c=-1\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/kparam\?b=open\&c=-1\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_K) -src '$(JSON)/sps-p\?b=open\&c=-1\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/kparam\?b=open\&c=-1\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)

img-kparam-p: json-p
	parallel $(ENV) python3 $(UTIL_K) -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/kparam\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_K) -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/kparam\?b=periodic\&c=10\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_K) -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/kparam\?b=periodic\&c=10\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)

img-xv: json
	parallel $(ENV) python3 $(UTIL_XV) -src '$(JSON)/sps-p\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/xv\?b=open\&c=-1\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_XV) -src '$(JSON)/sps-p\?b=open\&c=-1\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/xv\?b=open\&c=-1\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_XV) -src '$(JSON)/sps-p\?b=open\&c=-1\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/xv\?b=open\&c=-1\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)

img-xv-p: json-p
	parallel $(ENV) python3 $(UTIL_XV) -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/xv\?b=periodic\&c=10\&d=none\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_XV) -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/xv\?b=periodic\&c=10\&d=static\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)
	parallel $(ENV) python3 $(UTIL_XV) -src '$(JSON)/sps-p\?b=periodic\&c=10\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.json' \
	  -out '$(DEV_IMG)/xv\?b=periodic\&c=10\&d=dynamic\&g=$(MAXGEN)\&k={1}\&s={2}.png' ::: $(INITS) ::: $(SEEDS)

img: img-energy img-kparam img-xv
img-p: img-energy-p img-kparam-p img-xv-p

dev: html img
	cp -r css $(DEV)
	cp -r js $(DEV)

dev-p: html-p img-p

public: html img
	cp -r css $(PUBLIC)
	cp -r js $(PUBLIC)
	cp -r img $(PUBLIC)
	./gen_index.sh
	mv sps-p\?* $(PUBLIC)
	mv index.html $(PUBLIC)

clean:
	make -C src clean
	rm *.html
