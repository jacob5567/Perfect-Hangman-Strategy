// File: HangmanStrategy.c
// Jacob Faulk

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <omp.h>

using namespace std;

#define RESERVED_SIZE 10000
#define INPUT_FILE "dictionary.txt"
#define OUTPUT_FILE "results_small.txt"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: <executable> <number of threads>";
        return 1;
    }
    int thread_count = atoi(argv[1]);

    double time1 = omp_get_wtime();

    vector<string> words;
    string wordBuffer;
    words.reserve(RESERVED_SIZE);
    ifstream dictionaryFile;
    dictionaryFile.open(INPUT_FILE);
    int i = 0;
    if (dictionaryFile.is_open())
    {
        while (!dictionaryFile.eof())
        {
            getline(dictionaryFile, wordBuffer);
            if (!wordBuffer.empty() && wordBuffer[wordBuffer.size() - 1] == '\r')
                wordBuffer.erase(wordBuffer.size() - 1);
            words.push_back(wordBuffer);
            i++;
        }
    }
    else
    {
        std::cout << "Unable to open file." << std::endl;
        return 1;
    }

    dictionaryFile.close();

    double time2 = omp_get_wtime();

    // Working through words
#pragma omp parallel for num_threads(thread_count)
    for (int wordIndex = 0; wordIndex < words.size(); wordIndex++)
    {
        int len = words[wordIndex].length();

        string revealedText = "";
        for (int i = 0; i < len; i++)
        {
            revealedText.push_back('_');
        }

        vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
        vector<char> guessed;
        int guesses = 0;

        while (revealedText.find('_') != string::npos)
        {

            vector<string> possibleWords;
            possibleWords.reserve(RESERVED_SIZE / 3);

            // all words it could be by length
            for (int i = 0; i < words.size(); i++)
            {
                // length check
                if (words[i].length() != len)
                    continue;

                // check if it contains a letter that was already guessed
                for (char g : guessed) // TODO change this to just check the previously guessed letter
                {
                    if (words[i].find(g) != string::npos)
                        continue;
                }

                // check specific letter locations
                for (int j = 0; j < len; j++)
                {
                    if (revealedText[j] != '_' && revealedText[j] != words[i][j])
                        continue;
                }

                possibleWords.push_back(words[i]);
            }

            // Check how many words would be possible if the letter is not in the words[wordIndex]
            map<char, int> possibleWordCount;
            for (char letter : letters)
            {
                possibleWordCount.insert(pair<char, int>(letter, 0));

                for (int j = 0; j < possibleWords.size(); j++)
                {
                    if (words[j].find(letter) == string::npos)
                    {
                        possibleWordCount.at(letter)++;
                    }
                }
            }

            // Select which letter minimizes this number
            map<char, int>::iterator itr;
            char smallestLetter = '_';
            int smallestCount = RESERVED_SIZE;
            for (itr = possibleWordCount.begin(); itr != possibleWordCount.end(); itr++)
            {
                if (itr->second < smallestCount)
                {
                    smallestLetter = itr->first;
                    smallestCount = itr->second;
                }
            }

            // Guess letter
            guessed.push_back(smallestLetter);
            for (int j = 0; j < 26; j++)
            {
                if (letters[j] == smallestLetter)
                {
                    letters.erase(letters.begin() + j);
                    break;
                }
            }

            // reveal letters
            bool correctGuess = false;
            for (int j = 0; j < len; j++)
            {
                if (words[wordIndex][j] == smallestLetter)
                {
                    revealedText[j] = smallestLetter;
                    correctGuess = true;
                }
            }
            if (!correctGuess)
            {
                guesses++;
            }
        }

#pragma omp critical
        {
            ofstream outfile;
            outfile.open(OUTPUT_FILE, ios::app);
            outfile << revealedText << ", " << guesses << std::endl;
            outfile.close();
        }
    }
    double time3 = omp_get_wtime();

    std::cout << "Read time: " << time2 - time1 << std::endl;
    std::cout << "Processing time: " << time3 - time2 << std::endl;

    return 0;
}
