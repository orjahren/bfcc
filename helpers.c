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

struct Node
{
    char data;
    struct Node *next;
};
struct Node *newLinkedList()
{
    struct Node *root = malloc(sizeof(struct Node));
    if (root == NULL)
    {
        printf("Unable to allocate memory for new LL node\n");
        exit(1);
    }
    root->next = NULL;
    return root;
}

int getListLength(struct Node *root)
{
    struct Node *ptr = root;
    int len = 0;
    while (ptr != NULL)
    {
        ptr = ptr->next;
        len++;
    }
    printf("Found len %d\n", len);
    return len;
}

struct Node *getPenultimateNode(struct Node *root)
{
    struct Node *ptr = root;
    struct Node *prev = NULL;
    while (ptr != NULL)
    {
        prev = ptr;
        ptr = ptr->next;
    }
    return prev;
}

struct Node *appendToLinkedList(struct Node *root, char data)
{
    struct Node *penultimate = getPenultimateNode(root);

    struct Node *tail = penultimate->next;

    struct Node *newNode = newLinkedList();
    if (tail == NULL)
    {
        penultimate->next = newNode;
    }
    else
    {
        tail->next = newNode;
    }

    return newNode;
}

void printLinkedList(struct Node *root)
{
    struct Node *ptr = root;
    printf("Linkedlist with root @ %x: { ", root);
    while (ptr != NULL)
    {
        char data = ptr->data;
        printf(" %d ", data);

        ptr = ptr->next;
    }
    printf(" }\n");
}

char popLinkedList(struct Node *root)
{
    int len = getListLength(root);
    // TODO: Is handling this case needed? Why not simply never pop empty lists?
    if (len == 0)
    {
        printf("\tWARN: Popped from empty list\n");
        return 0;
    }
    else if (len == 1)
    {
        char ret = root->data;
        free(root);
        return ret;
    }
    // At this point, length must be at least 2

    // First, find final and penultimate nodes
    struct Node *penultimate = getPenultimateNode(root);
    printf("Penultimate is @ mem adr %x\n", penultimate);
    struct Node *oldTail = penultimate->next;
    printf("Tail is @ mem adr %x\n", oldTail);

    if (oldTail == NULL || penultimate == NULL)
    {
        printf("Critical error in linked list popping :/\n");
        printLinkedList(root);
    }

    printf("Found pen and old tail\n");

    // Pop last from list. Decouple "oldTail".
    printf("Wiping 'next' of penultimate node\n");
    penultimate->next = NULL;
    printf("Persisting data from tail\n");
    char ret = oldTail->data;
    printf("Freeing tail\n");
    free(oldTail);
    printf("Popped finely :)\n");
    // Return removed data
    return ret;
}