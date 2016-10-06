#!/bin/bash

## This is the compile script

g++ -g -o dstak -Ofast -march=native -std=c++11 ass3.cpp gamestate.cpp getmoves.cpp minimax.cpp
