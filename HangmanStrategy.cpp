// File: HangmanStrategy.c
// Jacob Faulk

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// #include <omp.h>

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
    string word;
    cout << words.max_size() << endl;
    words.reserve(380000);
    ifstream dictionaryFile;
    dictionaryFile.open("words_alpha.txt");
    int i = 0;
    if (dictionaryFile.is_open())
    {
        while (!dictionaryFile.eof())
        {
            getline(dictionaryFile, word);
            if (!word.empty() && word[word.size() - 1] == '\r')
                word.erase(word.size() - 1);
            words.push_back(word);
            i++;
        }
    }
    else
    {
        cout << "Unable to open file." << endl;
        return 1;
    }

    dictionaryFile.close();

    return 0;
}
