# Paths for output.
SRC=src
PROTO=proto/bin
LOCAL=.
LOCAL_IMG=$(LOCAL)/img
DEV=../public_html
DEV_IMG=$(DEV)/img
PUBLIC=public
PUBLIC_IMG=$(PUBLIC)/img

HTML_PATH=$(DEV)
PROTO_PATH=$(PROTO)
IMG_PATH=$(DEV_IMG)

# Paths for utilities.
UTIL=utils
UTIL_ALL=$(UTIL)/main.py
UTIL_K=$(UTIL)/kparam.py
UTIL_ENERGY=$(UTIL)/energy.py
UTIL_XV=$(UTIL)/xv.py
UTIL_CLUSTERING=$(UTIL)/clustering.py

# CUI environment setting for matplotlib.
ENV=MPLBACKEND=Agg

# The number of threads.
NPROC=50

# Common settings
CYCLES=20
INITS=random
DYNAMICS=local-dynamic-discrete
# Multiple ways for dynamics, but most of them aren't used in our experiments. i.e. HE=global-static-discrete, DHE=local-dynamic-discrete
#DYNAMICS=none global-static-discrete global-dynamic-discrete local-static-discrete local-dynamic-discrete local-static-continuous local-dynamic-continuous

###################################
#####    Basic experiment     #####
###################################
MAXGEN=800000
PROB=0
PROB2=0
SEEDS=$(shell seq 5000)

###################################
##### Perturbation experiment #####
###################################
#MAXGEN=1700000
#PROB=-1
#PROB2=0
#SEEDS=$(shell seq 30000)

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

img-clustering: gen
	parallel $(ENV) python3 $(UTIL_CLUSTERING) \
		-src '$(PROTO_PATH)/b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&p1={3}\&p2={4}\&s={5}.bin' \
		-out '$(IMG_PATH)/clustering_b=open\&c=-1\&d={1}\&g=$(MAXGEN)\&k={2}\&p1={3}\&p2={4}\&s={5}.png' \
		::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

img-clustering-p: gen-p
	parallel $(ENV) python3 $(UTIL_CLUSTERING) \
		-src '$(PROTO_PATH)/b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.bin' \
		-out '$(IMG_PATH)/clustering_b=periodic\&c={1}\&d={2}\&g=$(MAXGEN)\&k={3}\&p1={4}\&p2={5}\&s={6}.png' \
		::: $(CYCLES) ::: $(DYNAMICS) ::: $(INITS) ::: $(PROB) ::: $(PROB2) ::: $(SEEDS)

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
