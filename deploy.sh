#!/bin/bash
rm build/airtzar-ng
clear
g++ --std=c++11 src/*.cpp -o build/airtzar-ng
build/airtzar-ng