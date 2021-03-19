#!/bin/sh
clear
gcc -g main.c `sdl2-config --libs` -lSDL2 -lSDL2_image -lSDL2_ttf -o game -lm
