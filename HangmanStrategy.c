// File: HangmanStrategy.c
// Jacob Faulk

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int thread_count;
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <number of threads> <input file>\n", argv[0]);
        return 1;
    }
    thread_count = strtol(argv[1], NULL, 10);
    char *filename = argv[2]; // I might need to free this later?
    printf("name: %s\n", filename);
    printf("num threads: %d\n", thread_count);


    return 0;
}
