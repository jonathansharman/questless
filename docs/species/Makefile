#!/bin/bash

all: species.svg

species.svg: species.dot
	dot -Tsvg species.dot -o species.svg

.PHONY: clean

clean:
	rm *.svg

