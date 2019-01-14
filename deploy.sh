#!/bin/bash
rm build/airetzar-ng
clear
g++ --std=c++11 src/*.cpp -o build/airetzar-ng
build/airetzar-ng -i mon0 -f example.csv -t 15