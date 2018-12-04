#!/bin/sh

rm ./diff_tree
g++ -g diff_tree.cpp -o diff_tree --std=c++14

./diff_tree ./input
