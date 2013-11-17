TEST_FILES = test/*.test.js

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

clean:
		node-gyp clean


.PHONY: clean test build
