@echo off

mkdir cmake-build-debug && cd cmake-build-debug
cmake -DCMAKE_BUILD_TYPE=Debug -GNinja ..
ninja
pause
