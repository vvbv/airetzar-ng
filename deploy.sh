#!/bin/bash
rm build/airetzar-ng
clear
g++ --std=c++11 src/*.cpp -o build/airetzar-ng
# build/airetzar-ng -i wlp2s0mon -f test/in.csv -t 5