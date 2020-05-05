#!/bin/bash

g++ HangmanStrategy.cpp -o hangman -fopenmp
for i in 1 2 4 8; do
    echo "Running with $i threads:"
    ./hangman $i
    rm results_small.txt
done
rm hangman
