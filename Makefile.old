SRC=src
JSON=json
IMG=img
PUBLIC=public
PUBLIC_IMG=public/img
PUBLIC_LOCAL=../public_html
PUBLIC_LOCAL_IMG=../public_html/img

ENV=MPLBACKEND=Agg

MAXGEN=100000
# RANGE=-0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2
RANGE = -0.8 -0.6 -0.4 -0.2 0.0 0.2 0.4 0.6 0.8 1.0 1.2
# RANGE = -0.8 -0.4 0.0 0.4 0.8 1.2

test-same-global:
	./src/generator-o -seed 0 -gen $(MAXGEN) -dynamic global -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=global\&i=0.html 
	./src/generator-o -seed 1 -gen $(MAXGEN) -dynamic global -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=global\&i=1.html 
	./src/generator-o -seed 2 -gen $(MAXGEN) -dynamic global -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=global\&i=2.html 
	./src/generator-o -seed 3 -gen $(MAXGEN) -dynamic global -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=global\&i=3.html 
	./src/generator-o -seed 4 -gen $(MAXGEN) -dynamic global -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=global\&i=4.html 

test-same-local:
	./src/generator-o -seed 0 -gen $(MAXGEN) -dynamic local -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=local\&i=0.html
	./src/generator-o -seed 1 -gen $(MAXGEN) -dynamic local -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=local\&i=1.html
	./src/generator-o -seed 2 -gen $(MAXGEN) -dynamic local -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=local\&i=2.html
	./src/generator-o -seed 3 -gen $(MAXGEN) -dynamic local -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=local\&i=3.html
	./src/generator-o -seed 4 -gen $(MAXGEN) -dynamic local -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=local\&i=4.html

test-same: test-same-global test-same-local

test-html:
	./src/generator-o -gen $(MAXGEN) > $(PUBLIC_LOCAL)/abpm=0.0,0.0,0.0,0.0\&b=open.html
	./src/generator-o -gen $(MAXGEN) -init random > $(PUBLIC_LOCAL)/abpm=random\&b=open.html
	./src/generator-o -gen $(MAXGEN) -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open.html
	./src/generator-o -gen $(MAXGEN) -dynamic global > $(PUBLIC_LOCAL)/abpm=0.0,0.0,0.0,0.0\&b=open\&d=global.html
	./src/generator-o -gen $(MAXGEN) -dynamic local > $(PUBLIC_LOCAL)/abpm=0.0,0.0,0.0,0.0\&b=open\&d=local.html
	./src/generator-o -gen $(MAXGEN) -dynamic global -init random > $(PUBLIC_LOCAL)/abpm=random\&b=open\&d=global.html 
	./src/generator-o -gen $(MAXGEN) -dynamic local -init random > $(PUBLIC_LOCAL)/abpm=random\&b=open\&d=local.html
	./src/generator-o -gen $(MAXGEN) -dynamic global -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=global.html 
	./src/generator-o -gen $(MAXGEN) -dynamic local -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=local.html

hoge:
	./src/generator-o -gen $(MAXGEN) -dynamic global -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=global.html 
	./src/generator-o -json -gen $(MAXGEN) -dynamic global -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=global.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open\&d=global.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open\&d=global.json -dynamic
	$(ENV) python3 utils/stackplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open\&d=global.json

test-json:
	./src/generator-o -json -gen $(MAXGEN) > $(PUBLIC_LOCAL)/abpm=0.0,0.0,0.0,0.0\&b=open.json
	./src/generator-o -json -gen $(MAXGEN) -init random > $(PUBLIC_LOCAL)/abpm=random\&b=open.json
	./src/generator-o -json -gen $(MAXGEN) -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open.json
	./src/generator-o -json -gen $(MAXGEN) -dynamic global > $(PUBLIC_LOCAL)/abpm=0.0,0.0,0.0,0.0\&b=open\&d=global.json
	./src/generator-o -json -gen $(MAXGEN) -dynamic local > $(PUBLIC_LOCAL)/abpm=0.0,0.0,0.0,0.0\&b=open\&d=local.json
	./src/generator-o -json -gen $(MAXGEN) -dynamic global -init random > $(PUBLIC_LOCAL)/abpm=random\&b=open\&d=global.json
	./src/generator-o -json -gen $(MAXGEN) -dynamic local -init random > $(PUBLIC_LOCAL)/abpm=random\&b=open\&d=local.json
	./src/generator-o -json -gen $(MAXGEN) -dynamic global -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=global.json
	./src/generator-o -json -gen $(MAXGEN) -dynamic local -init same > $(PUBLIC_LOCAL)/abpm=same\&b=open\&d=local.json

test-img:
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=0.0,0.0,0.0,0.0\&b=open.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=random\&b=open.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=0.0,0.0,0.0,0.0\&b=open\&d=global.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=0.0,0.0,0.0,0.0\&b=open\&d=local.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=random\&b=open\&d=global.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=random\&b=open\&d=local.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open\&d=global.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open\&d=local.json
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=0.0,0.0,0.0,0.0\&b=open.json -dynamic
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=random\&b=open.json -dynamic
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open.json -dynamic
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=0.0,0.0,0.0,0.0\&b=open\&d=global.json -dynamic
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=0.0,0.0,0.0,0.0\&b=open\&d=local.json -dynamic
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=random\&b=open\&d=global.json -dynamic
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=random\&b=open\&d=local.json -dynamic
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open\&d=global.json -dynamic
	$(ENV) python3 utils/logplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open\&d=local.json -dynamic
	$(ENV) python3 utils/stackplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=0.0,0.0,0.0,0.0\&b=open.json
	$(ENV) python3 utils/stackplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=random\&b=open.json
	$(ENV) python3 utils/stackplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open.json
	$(ENV) python3 utils/stackplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=0.0,0.0,0.0,0.0\&b=open\&d=global.json
	$(ENV) python3 utils/stackplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=0.0,0.0,0.0,0.0\&b=open\&d=local.json
	$(ENV) python3 utils/stackplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=random\&b=open\&d=global.json
	$(ENV) python3 utils/stackplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=random\&b=open\&d=local.json
	$(ENV) python3 utils/stackplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open\&d=global.json
	$(ENV) python3 utils/stackplot.py -src $(PUBLIC_LOCAL_IMG)/abpm=same\&b=open\&d=local.json

generator:
	make -C src generator

json: generator
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic global -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-o -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic local -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -json -cycle 20 '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -json -cycle 40 '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic global -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic global -json -cycle 20 '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic local -json '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(SRC)/generator-p -k2 '0.8 0.4 {1} {2}' -gen $(MAXGEN) -dynamic local -json -cycle 20 '>' '$(JSON)/abpm=0.8,0.4,{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)

img-static-energy: json
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)

img-dynamic-energy: json
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
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
	# parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
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
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=global\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/logplot.py -dynamic -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&d=local\&c=20.json' ::: $(RANGE) ::: $(RANGE)

img-kparam-only:
	# parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=global.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=open\&d=local.json' ::: $(RANGE) ::: $(RANGE)
	parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=20.json' ::: $(RANGE) ::: $(RANGE)
	# parallel $(ENV) python3 utils/stackplot.py -src $(JSON)/abpm=0.8,0.4,'{1},{2}\&b=periodic\&c=40.json' ::: $(RANGE) ::: $(RANGE)
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
