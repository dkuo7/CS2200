#!/bin/bash
rm output.txt
for a in {1,2,4}
do
    ./os-sim $a | tail -n 2 | head -n 1  >> output.txt
done
