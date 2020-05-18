#!/bin/bash
cd CK2ToEU4
rm -rf build
rm -rf Release-Linux
rm -rf ../Release
cmake -H. -Bbuild -DCMAKE_C_COMPILER=/usr/local/bin/gcc-9 -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-9 -DCMAKE_EXE_LINKER_FLAGS=-liconv
cmake --build build -- -j3 
mv Release-Linux ../Release
cd .. 
