#!/bin/bash
rm build/airetzar-ng
clear
g++ --std=c++11 src/*.cpp -o build/airetzar-ng
# build/airetzar-ng -i wlp2s0mon -f test/in.csv -t 5 -w 84:AA:9C:C0:3D:58,8A:F7:C7:D0:E5:5C
# build/airetzar-ng -i wlp2s0mon -f test/in2.csv -t 5 -ap 15