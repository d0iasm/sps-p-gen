SRC_DIR=.
CPP_DST_DIR=./build
PY_DST_DIR=../utils
INPUTS = $(wildcard $(SRC_DIR)/*.proto)

build: $(INPUTS)
	protoc -I=$(SRC_DIR) --cpp_out=$(CPP_DST_DIR) $(INPUTS)
	protoc -I=$(SRC_DIR) --python_out=$(PY_DST_DIR) $(INPUTS)

clean:
	rm $(CPP_DST_DIR)/*
	rm $(PY_DST_DIR)/*_pb2.py

clean-data:
	rm ./bin/*

.PHONY: build
