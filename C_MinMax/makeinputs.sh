#!/bin/bash

cp ./src/geninputs $SCRATCH
cd $SCRATCH
mkdir -p inputs
./geninputs 10000000 >> ./inputs/file01
