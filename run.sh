#!/bin/bash

g++ HangmanStrategy.cpp -o hangman -fopenmp
./hangman 4
rm hangman
