#!/bin/bash

# Copy converter globals
cp "Data_Files/ReadMe.txt" "Release-Linux/ReadMe.txt"
cp "Data_Files/FAQ.txt" "Release-Linux/FAQ.txt"
cp "Data_Files/after_converting.txt" "Release-Linux/after_converting.txt"
cp "Data_Files/license.txt" "Release-Linux/license.txt"

# Copy configurables
rm -rf "Release-Linux/configurables"
cp -r "Data_Files/configurables" "Release-Linux/configurables"
