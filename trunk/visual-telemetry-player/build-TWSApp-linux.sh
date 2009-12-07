#!/bin/bash

g++ \
src/common/*.cpp \
src/client/*.cpp \
-ftree-vectorize \
-Isrc/client \
-Isrc/common \
-DGL_GLEXT_PROTOTYPES `sdl-config --cflags --libs` \
-lm -lglut -lGLU -lGL -lini \
-o TWSApp-linux

