#!/bin/bash

# 빌드 타입을 인자로 받습니다. 기본값은 "Release"입니다.
BUILD_TYPE=${1:-Release}

# CMake 호출
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

# 빌드 수행
make