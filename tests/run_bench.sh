#!/bin/bash

bazel build ... --cxxopt='-DNDEBUG'
../bazel-bin/tests/app_bench

