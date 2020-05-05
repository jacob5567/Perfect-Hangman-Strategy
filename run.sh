#!/bin/bash

rm results.txt
g++ HangmanStrategy.cpp -o hangman -fopenmp
./hangman 8
rm hangman
