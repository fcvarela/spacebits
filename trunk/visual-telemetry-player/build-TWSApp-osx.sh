#!/bin/bash

/Developer/usr/bin/llvm-g++-4.2\
    -O4\
    -fomit-frame-pointer\
    -msse3\
    -arch i386\
    -fast\
    -ftree-vectorize\
    -Isrc/client\
    -Isrc/common\
    -lobjc\
    -lini\
    -framework Cocoa\
    -framework OpenGL\
    -framework SDL\
    -o TWSApp-osx\
    src/client/*.cpp\
    src/common/*.cpp\
    src/client/osx/SDLMain.m
