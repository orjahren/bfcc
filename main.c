#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.c"

#define MEMORY_BYTES 30000 // 30_000
#define DEBUG 0

void interpretBfSource(char *sourceCode, char *bfMemory)
{
    int sourceLen = strlen(sourceCode);
    if (DEBUG)
    {
        printf("Source has %d chars\n", sourceLen);
    }

    // Precompute jumps from [ to its ] and vice-verca
    // TODO: This will be very sparse. Should consolidate memory usage?
    int jumpMap[sourceLen];
    for (int i = 0; i < sourceLen; i++)
    {
        // Init to -1 to flag bugs more easily
        jumpMap[i] = -1;
    }
    // TODO: Optimize this
    for (int i = 0; i < sourceLen; i++)
    {
        if (sourceCode[i] == '[')
        {
            if (DEBUG)
                printf("Found [ at pos %d\n", i);
            int othersFound = 0;
            for (int scanIdx = i + 1; scanIdx < sourceLen; scanIdx++)
            {
                if (sourceCode[scanIdx] == '[')
                {
                    othersFound++;
                }
                else if (sourceCode[scanIdx] == ']')
                {
                    if (othersFound == 0)
                    {
                        if (DEBUG)
                            printf("Its jump to ] is at pos %d\n", scanIdx);
                        jumpMap[i] = scanIdx;
                        jumpMap[scanIdx] = i;
                        break;
                    }
                    else
                    {
                        othersFound--;
                    }
                }
            }
        }
    }
    if (DEBUG)
    {
        printf("Precomputed jump map:\n{ ");
        for (int i = 0; i < sourceLen; i++)
        {
            printf(" %d ", jumpMap[i]);
        }
        printf("}\n");
    }

    int memIdx = 0;
    int instructionIndex = 0;

    do
    {
        if (instructionIndex < 0)
        {
            printf("CRITICAL: Illegal state, instruction pointer is %d\n", instructionIndex);
            exit(1);
        }

        char dontIncrementIpr = 0;
        // Not strictly an operator, might also be ignored symbol
        char operator = sourceCode[instructionIndex];
        if (DEBUG)
            printf("\t\t--- Instruction idx: %d, mem idx: %d, operator at idx: %c --- \n", instructionIndex, memIdx, operator);

        switch (operator)
        {
        case '<': // < decreases memory pointer, or moves the pointer to the left 1 block.
            memIdx--;
            break;
        case '>': // > increases memory pointer, or moves the pointer to the right 1 block.
            memIdx++;
            break;
        case '+': // + increases value stored at the block pointed to by the memory pointer
            bfMemory[memIdx]++;
            break;
        case '-': // - decreases value stored at the block pointed to by the memory pointer
            bfMemory[memIdx]--;
            break;
        case '[': // [ like c while(cur_block_value != 0) loop.
            if (bfMemory[memIdx] == 0)
                instructionIndex = jumpMap[instructionIndex];
            break;
        case ']': // ] if block currently pointed to's value is not zero, jump back to [
            if (bfMemory[memIdx] != 0)
                instructionIndex = jumpMap[instructionIndex];
            break;
        case ',': // , like c getchar(). input 1 character.
            bfMemory[memIdx] = getchar();
            break;
        case '.': // . like c putchar(). print 1 character to the console
            putchar(bfMemory[memIdx]);
            break;

        // Simply ignore non-supported chars.
        default:
            if (DEBUG)
                printf("Ignoring char %c\n", operator);
        }
    } while (instructionIndex++ < sourceLen);
}

int main(int argc, char **argv)
{

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