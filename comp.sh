#!/bin/sh
clear
echo -----------------------
echo -----------------------
echo -----------------------
gcc -g main.c `sdl2-config --libs` -lSDL2 -lSDL2_image -lSDL2_ttf -o vn -lm
