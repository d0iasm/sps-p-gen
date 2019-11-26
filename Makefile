SRC=src
PROTO=proto/bin
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
UTIL_CLUSTERING=$(UTIL)/clustering.py

HTML_PATH=$(DEV)
PROTO_PATH=$(PROTO)
IMG_PATH=$(DEV_IMG)

ENV=MPLBACKEND=Agg

NPROC=50

MAXGEN=1700000
# for experiment of local-static-discrete
#MAXGEN=800000
CYCLES=20
#DYNAMICS=none global-static-discrete global-dynamic-discrete local-static-discrete local-dynamic-discrete local-static-continuous local-dynamic-continuous
#DYNAMICS=none global-static-discrete global-dynamic-discrete local-static-discrete local-dynamic-discrete
DYNAMICS=local-dynamic-discrete
INITS=random
#PROB=-1 0 1 5 10 20 30 40 50 60 70 80
PROB=-1
PROB2=0
#SEEDS=0
#SEEDS=$(shell seq 1000)
SEEDS=$(shell seq 5000)
#SEEDS=$(shell seq 9990 9999)

generator:
	make -C src generator

generator-p:
	make -C src periodic

kano: generator
	parallel $(SRC)/generator-o -path_html $(HTML_PATH) -path_proto $(PROTO_PATH) -dynamic '{1}' -gen $(MAXGEN) -k2 0.8 0.4 0.6 -0.6 ::: $(DYNAMICS)
	parallel $(SRC)/generator-p -path_html $(HTML_PATH) -path_proto $(PROTO_PATH) -cycle '{1}' -dynamic '{2}' -gen $(MAXGEN) -k2 0.8 0.4 0.6 -0.6 ::: $(CYCLES) ::: $(DYNAMICS)

gen: generator
	parallel $(SRC)/generator-o -path_html $(HTML_PATH) -path_proto $(PROTO_PATH) -dynamic '{1}' -gen $(MAXGEN) -init '{2}' -p1 '{3}' -p2 '{4}' -seed '{5}' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

gen-p: generator-p
	parallel -j $(NPROC) $(SRC)/generator-p -path_html $(HTML_PATH) -path_proto $(PROTO_PATH) -path_proto $(PROTO_PATH) \
		-cycle '{1}' -dynamic '{2}' -gen $(MAXGEN) -init '{3}' -p1 '{4}' -p2 '{5}' -seed '{6}' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-all: gen
	parallel $(ENV) python3 $(UTIL_ALL) \
		-src '$(PROTO_PATH)/b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&p1={3}\&p2={4}\&s={5}.bin' \
		-out '$(IMG_PATH)/b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&p1={3}\&p2={4}\&&s={5}.png' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-all-p: gen-p
	parallel -j $(NPROC) $(ENV) python3 $(UTIL_ALL) \
		-src '$(PROTO_PATH)/b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.bin' \
		-out '$(IMG_PATH)/b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.png' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-energy: gen
	parallel $(ENV) python3 $(UTIL_ENERGY) \
		-src '$(PROTO_PATH)/b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&p1={3}\&p2={4}\&s={5}.bin' \
		-out '$(IMG_PATH)/energy_b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&p1={3}\&p2={4}\&&s={5}.png' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-energy-p: gen-p
	parallel $(ENV) python3 $(UTIL_ENERGY) \
		-src '$(PROTO_PATH)/b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.bin' \
		-out '$(IMG_PATH)/energy_b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.png' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-kparam: gen
	parallel $(ENV) python3 $(UTIL_K) \
		-src '$(PROTO_PATH)/b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&p1={3}\&p2={4}\&s={5}.bin' \
		-out '$(IMG_PATH)/kparam_b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&p1={3}\&p2={4}\&&s={5}.png' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-kparam-p: gen-p
	parallel $(ENV) python3 $(UTIL_K) \
		-src '$(PROTO_PATH)/b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.bin' \
		-out '$(IMG_PATH)/kparam_b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.png' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-xv: gen
	parallel $(ENV) python3 $(UTIL_XV) \
		-src '$(PROTO_PATH)/b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&p1={3}\&p2={4}\&s={5}.bin' \
		-out '$(IMG_PATH)/xv_b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&p1={3}\&p2={4}\&&s={5}.png' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-xv-p: gen-p
	parallel $(ENV) python3 $(UTIL_XV) \
		-src '$(PROTO_PATH)/b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.bin' \
		-out '$(IMG_PATH)/xv_b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.png' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

# TODO: make open boundary version.
img-clustering-p: gen-p
	parallel $(ENV) python3 $(UTIL_CLUSTERING) \
		-src '$(PROTO_PATH)/b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.bin' \
		-out '$(IMG_PATH)/clustering_b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.png' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img: img-energy img-kparam img-xv
img-p: img-energy-p img-kparam-p img-xv-p

dev: gen img-all
	cp -r css $(DEV)
	cp -r js $(DEV)
	rm js/data/*

dev-p: gen-p img-all-p
	cp -r css $(DEV)
	cp -r js $(DEV)
	rm js/data/*

dev-all: dev dev-p

public: dev-all
	cp -r css $(PUBLIC)
	cp -r js $(PUBLIC)
	rm js/data/*
	cp -r img $(PUBLIC)
	./gen_index.sh
	mv *.html $(PUBLIC)

clean:
	make -C src clean
	rm js/data/*
