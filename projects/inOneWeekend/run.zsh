#!/bin/zsh

cmake --build build/
./build/inOneWeekend > result/$1.ppm

