#!/bin/bash

bazel build ... -s --cxxopt='-DNDEBUG -fPIC -lrt -D_GLIBCXX_USE_CXX11_ABI=0 -lpthread -g  --std=c++17 -static'
valgrind ../bazel-bin/tests/app_bench

