#!/bin/bash
set -euo pipefail

gcc -Wall HangmanStrategy.c -o hangman -fopenmp
./hangman 4 words_alpha.txt
rm hangman
