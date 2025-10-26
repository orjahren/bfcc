#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.c"

#define LINE_LENGTH 300
#define MEMORY_BYTES 30000 // 30_000
#define DEBUG 1

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
    int loopStartIdx = -1;
    char curVal;

    // TODO: Trenger man egt 2 lister?? Hva med å bare ha en stack hvor man lagrer [-posisjoner, og så bare popper man stacken for hver ] og går til den lagrede posisjonen
    // We represent BF loops with symmetric lists, where each index in the lists correspond to loop indeces.
    struct Node *loopStarts = NULL; // Only instanciate the list if we have data for it.
    struct Node *loopEnds = NULL;   // Only instanciate the list if we have data for it.

    for (int i = 0; i < sourceLen; i++)
    {
        // Not strictly an operator, might also be ignored symbol
        char operator = sourceCode[i];
        if (DEBUG)
            printf("Char is %c\n", operator);

        switch (operator)
        {

        case '<': // < decreases memory pointer, or moves the pointer to the left 1 block.
            // TODO: How should we handle wrapping around the memory?
            memIdx = (memIdx - 1) % MEMORY_BYTES;
            break;
        case '>': // > increases memory pointer, or moves the pointer to the right 1 block.
            // TODO: How should we handle wrapping around the memory?
            memIdx = (memIdx + 1) % MEMORY_BYTES;
            break;
        case '+': // + increases value stored at the block pointed to by the memory pointer
            bfMemory[memIdx]++;
            break;
        case '-': // - decreases value stored at the block pointed to by the memory pointer
            bfMemory[memIdx]--;
            break;
        case '[': // [ like c while(cur_block_value != 0) loop.
            printCurrentMemory(bfMemory, memIdx);

            printf("Resultatet er: %c\n", bfMemory[memIdx] != 0);

            if (bfMemory[memIdx] != 0)
            {

                printf("Mem is not 0, init loop\n");
                if (loopStarts == NULL)
                {
                    loopStarts = newLinkedList();
                    loopStarts->data = memIdx;
                }
                else
                {
                    appendToLinkedList(loopStarts, memIdx);
                }
            }
            else
            {
                printf("No loop, will pop lists\n");
                popLinkedList(loopStarts);
                popLinkedList(loopEnds);
            }
            printf("Breaker\n");
            break;
        case ']': // ] if block currently pointed to's value is not zero, jump back to [
            if (bfMemory[memIdx] != 0)
            {
                printf("Mem is not 0, loop back\n");
                struct Node *jumpTo = getPenultimateNode(loopStarts);
                memIdx = jumpTo->data;
            }
            else
            {
                printf("Mem IS 0, pop loop lists\n");
                popLinkedList(loopStarts);
                popLinkedList(loopEnds);
            }
            break;
        case ',': // , like c getchar(). input 1 character.
            bfMemory[memIdx] = getchar();
            break;
        case '.': // . like c putchar(). print 1 character to the console
            char memVal = bfMemory[memIdx];
            if (DEBUG)
                printCurrentMemory(bfMemory, memIdx);
            putchar(memVal);
            break;

        // Simply ignore non-supported chars.
        default:
            if (DEBUG)
                printf("Ignoring char %c\n", operator);
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