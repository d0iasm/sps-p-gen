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

MAXGEN=400000
#CYCLES=10 20 50 100
CYCLES=20
INITS=zero random
#SEEDS=0
SEEDS=0 1 2 3 4 5 6 7
#DYNAMICS=none global-static-discrete global-dynamic-discrete local-static-discrete local-dynamic-discrete local-static-continuous local-dynamic-continuous
#DYNAMICS=none global-static-discrete global-dynamic-discrete local-static-discrete local-dynamic-discrete
DYNAMICS=local-dynamic-discrete

generator:
	make -C src generator

kano: generator
	parallel $(SRC)/generator-o -dynamic '{1}' -gen $(MAXGEN) -k2 0.8 0.4 0.6 -0.6 '>' \
		'$(DEV)/sps-p\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k=0.8,0.4,0.6,-0.6\&s=0.html' ::: $(DYNAMICS)
	parallel $(SRC)/generator-p -cycle '{1}' -dynamic '{2}' -gen $(MAXGEN) -k2 0.8 0.4 0.6 -0.6 '>' \
		'$(DEV)/sps-p\?b=periodic\&c=$(CYCLE)\&d={1}\&g=$(MAXGEN)\&k=0.8,0.4,0.6,-0.6\&s=0.html' ::: $(CYCLES) ::: $(DYNAMICS)

html: generator
	parallel $(SRC)/generator-o -dynamic '{1}' -gen $(MAXGEN) -init '{2}' -seed '{3}' '>' \
		'$(DEV)/sps-p\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.html' ::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

html-p: generator
	parallel $(SRC)/generator-p -cycle '{1}' -dynamic '{2}' -gen $(MAXGEN) -init '{3}' -seed '{4}' '>' \
		'$(DEV)/sps-p\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&s={4}.html' ::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

json: generator
	parallel $(SRC)/generator-o -json -dynamic '{1}' -gen $(MAXGEN) -init '{2}' -seed '{3}' '>' \
		'$(JSON)/sps-p\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.json' ::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

json-p: generator
	parallel $(SRC)/generator-p -json -cycle '{1}' -dynamic '{2}' -gen $(MAXGEN) -init '{3}' -seed '{4}' '>' \
		'$(JSON)/sps-p\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&s={4}.json' :: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

img-energy: json
	parallel $(ENV) python3 $(UTIL_ENERGY) -src '$(JSON)/sps-p\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.json' \
		-out '$(DEV_IMG)/energy\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.png' ::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

img-energy-p: json-p
	parallel $(ENV) python3 $(UTIL_ENERGY) -src '$(JSON)/sps-p\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&s={4}.json' \
		-out '$(DEV_IMG)/energy\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&s={4}.png' ::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

img-kparam: json
	parallel $(ENV) python3 $(UTIL_K) -src '$(JSON)/sps-p\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.json' \
		-out '$(DEV_IMG)/kparam\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.png' ::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

img-kparam-p: json-p
	parallel $(ENV) python3 $(UTIL_K) -src '$(JSON)/sps-p\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&s={4}.json' \
		-out '$(DEV_IMG)/kparam\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&s={4}.png' ::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

img-xv: json
	parallel $(ENV) python3 $(UTIL_XV) -src '$(JSON)/sps-p\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.json' \
		-out '$(DEV_IMG)/xv\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.png' ::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

img-xv-p: json-p
	parallel $(ENV) python3 $(UTIL_XV) -src '$(JSON)/sps-p\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&s={4}.json' \
		-out '$(DEV_IMG)/xv\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&s={4}.png' ::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

img: img-energy img-kparam img-xv
img-p: img-energy-p img-kparam-p img-xv-p

dev: html img
	cp -r css $(DEV)
	cp -r js $(DEV)

dev-p: html-p img-p

dev-all: dev dev-p

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
