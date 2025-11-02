#include <stdio.h>

#define DEBUG_HELPERS 0

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

struct Node
{
    int data;
    struct Node *next;
};
struct Node *newNode(int data)
{
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL)
    {
        printf("Unable to allocate memory for new node\n");
        exit(1);
    }
    node->next = NULL;
    node->data = data;
    return node;
}

void printStack(struct Node *root)
{
    struct Node *ptr = root;
    if (!DEBUG_HELPERS)
        return;
    printf("Stack with root @ 0x%x: { ", root);
    while (ptr != NULL)
    {
        int data = ptr->data;
        printf(" %d ", data);

        ptr = ptr->next;
    }
    printf(" }\n");
}

int peekStack(struct Node *root)
{
    if (DEBUG_HELPERS)
        printf("Peeker stack 0x%x\n", root);
    if (root == NULL)
    {
        printf("Illegal state: Attempted to peek empty stack.\n");
        exit(1);
    }
    printStack(root);
    return root->data;
}

struct Node *popStack(struct Node *root)
{
    if (root == NULL)
    {
        printf("Illegal state: Attempted to pop empty stack.\n");
        exit(1);
    }
    struct Node *ret = root->next;
    free(root);
    return ret;
}

struct Node *pushStack(struct Node *root, struct Node *nn)
{
    if (DEBUG_HELPERS)
        printf("Skal pushe til 0x%x\n", root);
    if (root == NULL)
    {
        return nn;
    }
    nn->next = root;
    return nn;
}
