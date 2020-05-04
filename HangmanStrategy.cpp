// File: HangmanStrategy.c
// Jacob Faulk

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: <executable> <number of threads>";
        return 1;
    }
    int thread_count = atoi(argv[1]);

    cout << "num threads: " << thread_count << endl;

    vector<string> words;
    string wordBuffer;
    cout << words.max_size() << endl;
    words.reserve(380000);
    ifstream dictionaryFile;
    dictionaryFile.open("words_alpha.txt");
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
        cout << "Unable to open file." << endl;
        return 1;
    }

    dictionaryFile.close();

    // Working through words
    for (string word : words)
    {
        int len = word.length();
        // TODO number of guesses + extract this whole thing into a loop

        string revealedText = "";
        for (int i = 0; i < len; i++)
        {
            revealedText.push_back('_');
        }

        vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
        vector<char> guessed;
        vector<string> possibleWords;
        possibleWords.reserve(10000);

        // all words it could be by length
        #pragma omp parallel for num_threads(thread_count)
        for (int i = 0; i < words.size(); i++)
        {
            // length check
            if (words[i].length() != len)
                continue;

            // check if it contains a letter that was already guessed
            for (char g : guessed) // TODO change this to just check the previously guessed letter
            {
                if(words[i].find(g))
                    continue;
            }

            // check specific letter locations
            for (int j = 0; j < len; j++)
            {
                if(revealedText[j] != '_' && revealedText[j] != words[i][j])
                    continue;
            }

            // add word as possible
            #pragma omp critical
            {
                possibleWords.push_back(words[i]);
            }
        }
    }

    return 0;
}
