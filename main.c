#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.c"

#define LINE_LENGTH 300
#define MEMORY_BYTES 30000 // 30_000
#define DEBUG 0

void printCurrentMemory(char *bfMemory, int memIdx)
{
    printf("Current memory is %d, at idx %d\n", bfMemory[memIdx], memIdx);
}
void interpretBfSource(char *sourceCode, char *bfMemory)
{
    int sourceLen = strlen(sourceCode);
    if (DEBUG)
    {
        printf("Source has %d chars\n", sourceLen);
    }

    int memIdx = 0;
    int instructionIndex = 0;

    struct Node *instructionIdxStack = NULL;

    // for (int i = 0; i < sourceLen; i++)
    while (instructionIndex < sourceLen)
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
            // TODO: How should we handle wrapping around the memory?
            memIdx--;
            // memIdx = (memIdx - 1) % MEMORY_BYTES;
            break;
        case '>': // > increases memory pointer, or moves the pointer to the right 1 block.
            // TODO: How should we handle wrapping around the memory?
            memIdx++;
            // memIdx = (memIdx + 1) % MEMORY_BYTES;
            break;
        case '+': // + increases value stored at the block pointed to by the memory pointer
            bfMemory[memIdx]++;
            break;
        case '-': // - decreases value stored at the block pointed to by the memory pointer
            bfMemory[memIdx]--;
            break;
        case '[': // [ like c while(cur_block_value != 0) loop.
            if (DEBUG)
                printCurrentMemory(bfMemory, memIdx);

            if (DEBUG)
                printf("Resultatet er: %d\n", bfMemory[memIdx] != 0);

            if (bfMemory[memIdx] != 0)
            {

                if (DEBUG)
                    printf("Mem is not 0, init loop for instruction idx %d and mem idx %d\n", instructionIndex, memIdx);
                instructionIdxStack = pushStack(instructionIdxStack, newNode(instructionIndex));
            }
            else
            {
                if (DEBUG)
                    printf("No loop\n");
                // instructionIdxStack = popStack(instructionIdxStack);
            }
            if (DEBUG)
                printf("Breaker\n");
            break;
        case ']': // ] if block currently pointed to's value is not zero, jump back to [
            if (bfMemory[memIdx] != 0)
            {
                if (DEBUG)
                    printf("Mem is not 0 (%d), loop back\n", bfMemory[memIdx]);
                instructionIndex = peekStack(instructionIdxStack);
                if (DEBUG)
                    printf("Set instruciton idx to %d\n", instructionIndex);
                dontIncrementIpr = 1;
                instructionIdxStack = popStack(instructionIdxStack);
            }
            else
            {
                // TODO: Denne skal HOPPE FREM til sammensvarende ]
                if (DEBUG)
                    printf("Mem IS 0, pop jump stack\n");
                instructionIdxStack = popStack(instructionIdxStack);
            }
            break;
        case ',': // , like c getchar(). input 1 character.
            bfMemory[memIdx] = getchar();
            break;
        case '.': // . like c putchar(). print 1 character to the console
            char memVal = bfMemory[memIdx];
            if (DEBUG)
                printf("Putchar: %d/0x%x\n", memVal, memVal);
            putchar(memVal);
            break;

        // Simply ignore non-supported chars.
        default:
            if (DEBUG)
                printf("Ignoring char %c\n", operator);
        }
        if (!dontIncrementIpr)
            instructionIndex++;
        if (instructionIndex >= sourceLen)
            break;
    }
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