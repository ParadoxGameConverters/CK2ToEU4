#!/bin/bash

export CC=/usr/bin/gcc-11 &&
export CXX=/usr/bin/g++-11 &&

rm -rf Release/ &&
mkdir Release &&
cd CK2ToEU4 &&
rm -rf build &&
rm -rf Release-Linux &&
cmake -H. -Bbuild &&
cmake --build build -- -j40 &&
mv Release-Linux ../Release/CK2ToEU4 &&
cd .. &&

cd Fronter.NET &&
dotnet publish -p:PublishProfile=linux-x64 --output:"../Release" &&
cd .. &&

cp CK2ToEU4/Data_Files/*yml Release/Configuration/ &&
cp CK2ToEU4/Data_Files/fronter*txt Release/Configuration/ &&

tar -cjf CK2ToEU4-dev-release.tar.bz2 Release
