#!/bin/bash

mkdir twsapp
rsync --exclude .svn --exclude .DS_Store -av Data twsapp/
mv TWSApp-osx twsapp
mv TWSServer-osx twsapp
mv TWSApp-linux twsapp
mv TWSServer-linux twsapp
tar -czf twsapp.tgz twsapp
rm -rf twsapp
