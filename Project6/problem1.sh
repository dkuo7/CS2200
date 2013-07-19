#!/bin/bash
for a in {1,2,4,8,16}
do
    ./os-sim $a | tail -n 2 | head -n 1
done
