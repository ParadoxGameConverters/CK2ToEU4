#!/bin/bash

export CC=/usr/bin/gcc-10 &&
export CXX=/usr/bin/g++-10 &&

rm -rf Release/ &&
cd Fronter &&
./build_linux.sh &&
mv Release ../ &&
cd ../CK2ToEU4 &&
rm -rf build &&
rm -rf Release-Linux &&
cmake -H. -Bbuild &&
cmake --build build -- -j3 &&
mv Release-Linux ../Release/CK2ToEU4 &&
cd .. &&

cp CK2ToEU4/Data_Files/*yml Release/Configuration/ &&
cp CK2ToEU4/Data_Files/fronter*txt Release/Configuration/ &&

tar -cjf CK2ToEU4-dev-release.tar.bz2 Release
