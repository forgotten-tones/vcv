INC=-I$(RACK_DIR) -I$(RACK_DIR)/include

all: build

build:
	BINDGEN_EXTRA_CLANG_ARGS="$(INC)" cargo build

