#!/bin/bash
for a in {8,6,4,2}
do
    ./os-sim 1 -r $a | tail -n 3 
done
