#!/bin/bash


if [[ ! $(sudo echo 0) ]]; then exit; fi

mkdir /usr/local/include/scannerpp
cp src/Scanner.hpp /usr/local/include/scannerpp

