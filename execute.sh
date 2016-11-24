#!/usr/bin/env bash

echo "Compiling..."
clang++ -std=c++0x -Wl,-rpath=/home/heykay/openblas/build/mymachine/lib -Wl,-rpath=/home/heykay/arma/build/mymachine/lib  -I/home/heykay/openblas/build/mymachine/include -I/home/heykay/arma/build/mymachine/include -o dpqs main.cpp -L/home/heykay/openblas/build/mymachine/lib -lopenblas -L/home/heykay/arma/build/mymachine/lib -larmadillo

echo "Done Compiling!"

echo "Running..."
./dpqs

echo "Done!"
echo "Plotting Results"
python plot_result.py
echo "Done"