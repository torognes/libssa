#!/bin/bash
./benchmark -N 1 -O -$3 -E -$4 -M $5 -c 10 -i $1 -d $2  -t SW -b 16 -s AVX2
