SRC=src
JSON=json
IMG=img
PUBLIC=public

ENV=MPLBACKEND=Agg

MAXGEN=100000
# RANGE=-0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2
RANGE = -0.8 -0.6 -0.4 -0.2 0.0 0.2 0.4 0.6 0.8 1.0 1.2
# RANGE = -0.8 -0.4 0.0 0.4 0.8 1.2

generator:
	make -C src generator

json: generator
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic global -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic local -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -json -cycle 20 '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -json -cycle 40 '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic global -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic global -json -cycle 20 '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic local -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic local -json -cycle 20 '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)

img-static-energy: json
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)

img-dynamic-energy: json
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)

img-energy: img-static-energy img-dynamic-energy

img-kparam: json
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)

img: img-kparam img-energy

img-energy-only:
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)

img-kparam-only:
	# parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)

img-only: img-kparam-only img-energy-only

html: generator
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) '>' 'abpm=0.8,0.4,{1},{2}\&b=open.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic global '>' 'abpm=0.8,0.4,{1},{2}\&b=open\&d=global.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic local '>' 'abpm=0.8,0.4,{1},{2}\&b=open\&d=local.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -cycle 20 '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&c=20.html' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -cycle 40 '>' 'abpm=0.8,0.4,{1},{2}\&b=periodic\&c=40.html' ::: $(RANGE) ::: $(RANGE)
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

# === Will remove the following functions. ===
csv:
	./gencsv.sh -open
	./gencsv.sh -periodic

csve:
	./gencsve.sh -open
	./gencsve.sh -periodic
