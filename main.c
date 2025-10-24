#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.c"

#define LINE_LENGTH 300
#define MEMORY_BYTES 30000 // 30_000
#define DEBUG 1

// TODO: Its probably possible to do this more efficiently. But as n is only 8, its probably fine.
char charIsInLanguage(char c)
{
    // BF only supports 8 operators. We will ignore all other chars.
    // https://gist.github.com/roachhd/dce54bec8ba55fb17d3a -> <>+-[],.
    char supportedChars[8] = {'<', '>', '+', '-', '[', ']', ',', '.'};
    for (int i = 0; i < 8; i++)
    {
        if (supportedChars[i] == c)
        {
            // If we find a match, we instantly return.
            return 1;
        }
    }
    return 0;
}

void interpretBfSource(char *sourceCode, char *memory)
{
    int sourceLen = strlen(sourceCode);
    if (DEBUG)
    {
        printf("Source has %d chars\n", sourceLen);
    }

    int memIdx = 0;
    for (int i = 0; i < sourceLen; i++)
    {
        char c = sourceCode[i];
        if (!charIsInLanguage(c))
        {
            if (DEBUG)
                printf("Skipping non-supported char %c\n", c);
            // Simply ignore non-supported chars.
            continue;
        }

        switch (c)
        {

            // < = decreases memory pointer, or moves the pointer to the left 1 block.
        case '<':
            if (DEBUG)
                printf("Switch case for char %c\n", c);
            break;
            // > = increases memory pointer, or moves the pointer to the right 1 block.
        case '>':
            if (DEBUG)
                printf("Switch case for char %c\n", c);
            break;
            // + = increases value stored at the block pointed to by the memory pointer
        case '+':
            if (DEBUG)
                printf("Switch case for char %c\n", c);
            break;
            // - = decreases value stored at the block pointed to by the memory pointer
        case '-':
            if (DEBUG)
                printf("Switch case for char %c\n", c);
            break;
            // [ = like c while(cur_block_value != 0) loop.
        case '[':
            if (DEBUG)
                printf("Switch case for char %c\n", c);
            break;
            // ] = if block currently pointed to's value is not zero, jump back to [
        case ']':
            if (DEBUG)
                printf("Switch case for char %c\n", c);
            break;
            // , = like c getchar(). input 1 character.
        case ',':
            if (DEBUG)
                printf("Switch case for char %c\n", c);
            break;
            // . = like c putchar(). print 1 character to the console
        case '.':
            if (DEBUG)
                printf("Switch case for char %c\n", c);
            char memVal = memory[memIdx];
            printf("Memval is %c\n", memVal);
            putchar(memVal);
            break;

        default:
            // TODO: Do we really need to 'continue' above? How about just having this default be a NOP?
            printf("Illegal state in char switch statement for char %c\n", c);
            exit(1);
        }
    }
}

int main(int argc, char **argv)
{
    printf("Implementation is TODO!\n");

    char *fileName = argv[1];
    if (DEBUG)
        printf("Input file name is %s\n", fileName);

    char *bfMemory = initBrainFuckMemory(MEMORY_BYTES);

    char *fileContent = fileNameToContentString(fileName);

    if (DEBUG)
        printf("Leste følgende:\n%s\n", fileContent);

    if (DEBUG)
        printf("Will interpret.\n");
    interpretBfSource(fileContent, bfMemory);
    if (DEBUG)
        printf("Done :)\n");

    return 0;
}