@echo off

mkdir cmake-build-debug && cd cmake-build-debug
cmake -D CMAKE_C_COMPILER=gcc.exe -D CMAKE_CXX_COMPILER=g++.exe -D CMAKE_BUILD_TYPE=Debug -GNinja ..
ninja
pause
