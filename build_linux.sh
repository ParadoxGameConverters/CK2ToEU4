#!/bin/bash

rm -rf Release-Linux &&
cmake -H. -Bbuild &&
cmake --build build -- -j40
