#!/bin/bash

./os-sim 1 | tail -n 3 
./os-sim 1 -r 4 | tail -n 3 
./os-sim 1 -p | tail -n 3 
