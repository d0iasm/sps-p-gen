SRC_DIR=.
DST_DIR=./build
INPUTS = $(wildcard $(SRC_DIR)/*.proto)

build: $(INPUTS)
	protoc -I=$(SRC_DIR) --cpp_out=$(DST_DIR) $(INPUTS)

clean:
	rm $(DST_DIR)/*

clean-data:
	rm ./bin/*
