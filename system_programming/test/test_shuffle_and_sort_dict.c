#include <stdio.h>
#include <stdlib.h>

#include "shuffle_and_sort_dict.h"

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

#define NUM_OF_DICTS 1

unsigned char* InitArray(size_t* size)
{
    unsigned char* array = NULL;
    FILE* words = NULL;
    int c = '\0';
    size_t i = 0;
    size_t j = 0;

    words = fopen("/usr/share/dict/words", "r");

    while (EOF != fgetc(words))
    {
        ++i;
    }

    i *= NUM_OF_DICTS;

    *size = i;
    array = (unsigned char*)malloc(i * sizeof(unsigned char));

    for (; j < NUM_OF_DICTS; ++j)
    {
        fseek(words, 0, SEEK_SET);
        for (c = fgetc(words); EOF != c; c = fgetc(words))
        {
            array[--i] = (unsigned char)c;
        }
    }

    fclose(words);

    return array;
}

int main()
{
    size_t size = 0;

    free(InitArray(&size));

    return 0;
}
