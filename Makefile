TEST_FILES = test/*.test.js
BENCHMARK_FILES = benchmark/*.benchmark.js

all: test

build: clean configure compile

configure:
		node-gyp configure

compile: configure
		node-gyp build
		npm install .

debug: clean
		node-gyp configure -d build

test: build
		tap $(TEST_FILES)

benchmark: build
		for f in $(BENCHMARK_FILES); do node $$f; done;

clean:
		node-gyp clean


.PHONY: clean test build
