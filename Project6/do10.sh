#!/bin/bash
for a in {1..10}
do
    ./os-sim $a | tail -n 3 >> output.txt
done
