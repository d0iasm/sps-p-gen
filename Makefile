SRC=src
JSON=json
LOCAL=.
LOCAL_IMG=$(LOCAL)/img
DEV=../public_html
DEV_IMG=$(DEV)/img
PUBLIC=public
PUBLIC_IMG=$(PUBLIC)/img

UTIL=utils
UTIL_ALL=$(UTIL)/main.py
UTIL_K=$(UTIL)/stackplot.py
UTIL_ENERGY=$(UTIL)/logplot.py
UTIL_XV=$(UTIL)/loglogplot.py

HTML_PATH=$(DEV)
JSON_PATH=$(JSON)
IMG_PATH=$(DEV_IMG)

ENV=MPLBACKEND=Agg

MAXGEN=600000
#CYCLES=10 20 50 100
CYCLES=20
#DYNAMICS=none global-static-discrete global-dynamic-discrete local-static-discrete local-dynamic-discrete local-static-continuous local-dynamic-continuous
#DYNAMICS=none global-static-discrete global-dynamic-discrete local-static-discrete local-dynamic-discrete
DYNAMICS=local-dynamic-discrete
#INITS=zero random
INITS=random
#PROB=-1 0 1 5 10 20 30 40 50 60 70 80
PROB=-1
PROB2=0
#SEEDS=0
SEEDS=0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99

generator:
	make -C src generator

generator-p:
	make -C src periodic

kano: generator
	parallel $(SRC)/generator-o -path $(HTML_PATH) -dynamic '{1}' -gen $(MAXGEN) -k2 0.8 0.4 0.6 -0.6 ::: $(DYNAMICS)
	parallel $(SRC)/generator-p -path $(HTML_PATH) -cycle '{1}' -dynamic '{2}' -gen $(MAXGEN) -k2 0.8 0.4 0.6 -0.6 ::: $(CYCLES) ::: $(DYNAMICS)

#TODO: add probabilities p1 and p2.
html: generator
	parallel $(SRC)/generator-o -path $(HTML_PATH) -dynamic '{1}' -gen $(MAXGEN) -init '{2}' -seed '{3}' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

html-p: generator-p
	parallel $(SRC)/generator-p -path $(HTML_PATH) -cycle '{1}' -dynamic '{2}' -gen $(MAXGEN) -init '{3}' -p1 '{4}' -p2 '{5}' -seed '{6}' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

json: generator
	parallel $(SRC)/generator-o -json -path $(JSON_PATH) -dynamic '{1}' -gen $(MAXGEN) -init '{2}' -seed '{3}' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

json-p: generator-p
	parallel $(SRC)/generator-p -json -path $(JSON_PATH) -cycle '{1}' -dynamic '{2}' -gen $(MAXGEN) -init '{3}' -p1 '{4}' -p2 '{5}' -seed '{6}' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-all-p: json-p
	parallel $(ENV) python3 $(UTIL_ALL) \
		-src '$(JSON_PATH)/sps-p\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.json' \
		-out '$(IMG_PATH)/b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.png' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-energy: json
	parallel $(ENV) python3 $(UTIL_ENERGY) \
		-src '$(JSON_PATH)/sps-p\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.json' \
		-out '$(IMG_PATH)/energy\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.png' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

img-energy-p: json-p
	parallel $(ENV) python3 $(UTIL_ENERGY) \
		-src '$(JSON_PATH)/sps-p\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.json' \
		-out '$(IMG_PATH)/energy\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.png' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-kparam: json
	parallel $(ENV) python3 $(UTIL_K) \
		-src '$(JSON_PATH)/sps-p\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.json' \
		-out '$(IMG_PATH)/kparam\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.png' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

img-kparam-p: json-p
	parallel $(ENV) python3 $(UTIL_K) \
		-src '$(JSON_PATH)/sps-p\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.json' \
		-out '$(IMG_PATH)/kparam\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.png' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-xv: json
	parallel $(ENV) python3 $(UTIL_XV) \
		-src '$(JSON_PATH)/sps-p\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.json' \
		-out '$(IMG_PATH)/xv\?b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&s={3}.png' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(SEEDS)

img-xv-p: json-p
	parallel $(ENV) python3 $(UTIL_XV) \
		-src '$(JSON_PATH)/sps-p\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.json' \
		-out '$(IMG_PATH)/xv\?b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.png' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img: img-energy img-kparam img-xv
img-p: img-energy-p img-kparam-p img-xv-p

dev: html img
	cp -r css $(DEV)
	cp -r js $(DEV)

dev-p: html-p img-all-p

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
