#include <stdio.h>

// "file to string" borrowed from https://stackoverflow.com/a/174552
char *fileNameToContentString(char *fileName)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(fileName, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    if (!buffer)
    {
        printf("Unable to read file %s\n", fileName);
        exit(1);
    }
    return buffer;
}

char *initBrainFuckMemory(int numBytes)
{
    char *memPtr = malloc(sizeof(char) * numBytes);
    if (memPtr == NULL)
    {
        printf("Unable to allocate BF memory :(\n");
        exit(1);
    }
    // Initialize all cells to 0. TODO: Should use memset?
    for (int i = 0; i < numBytes; i++)
    {
        memPtr[i] = 0;
    }
    return memPtr;
}