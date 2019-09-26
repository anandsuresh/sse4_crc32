NODE_GYP = ./node_modules/.bin/node-gyp
MOCHA = ./node_modules/.bin/mocha
TEST_FILES = test/*.test.js
BENCHMARK_FILES = benchmark/*.benchmark.js

all: test

build: clean configure compile

configure:
		npm install .
		$(NODE_GYP) configure

compile: configure
		$(NODE_GYP) build

debug: clean
		$(NODE_GYP) configure -d build

test: build
		$(MOCHA) $(TEST_FILES)

benchmark: build
		for f in $(BENCHMARK_FILES); do node $$f; done;

clean:
		$(NODE_GYP) clean


.PHONY: clean build test
