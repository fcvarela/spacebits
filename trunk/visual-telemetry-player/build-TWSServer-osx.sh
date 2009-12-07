#!/bin/bash

g++ \
    -O3\
    -msse3\
    -arch i386\
    -Isrc/server\
    -Isrc/common\
    -I/usr/local/include/mysql++\
    -I/usr/local/mysql/include\
    -DTWS_SERVER_BUILD\
    -lmysqlpp\
    -lini\
    -o TWSServer-osx\
    src/server/*.cpp src/common/*.cpp
