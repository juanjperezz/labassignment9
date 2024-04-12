#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 23
// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};
struct Node
{
    struct RecordType record;
    struct Node* next;
};
// HashType
struct HashType
{
    struct Node** buckets;
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE;
}
struct Node* createNode(struct RecordType data)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode)
    {
        newNode->record = data;
        newNode->next = NULL;
    }
    return newNode;
}
void insertRecord(struct HashType* hashTable, struct RecordType data)
{
    int index = hash(data.id);
    struct Node* newNode = createNode(data);
    if (hashTable->buckets[index] == NULL)
    {
        hashTable->buckets[index] = newNode;
    }
    else
    {
        struct Node* current = hashTable->buckets[index];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to display records in the hash structure
void displayRecordsInHash(struct HashType* hashTable)
{
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        printf("Index %d -> ", i);
        struct Node* current = hashTable->buckets[i];
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->record.id, current->record.name, current->record.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}
// Function to free memory used by the hash table
void freeHashTable(struct HashType* hashTable)
{
    if (hashTable)
    {
        for (int i = 0; i < HASH_SIZE; ++i)
        {
            struct Node* current = hashTable->buckets[i];
            while (current != NULL)
            {
                struct Node* temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(hashTable->buckets);
        free(hashTable);
    }
}
// parses input file to an array of RecordType
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}
// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}
int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType));
    if (hashTable)
    {
        hashTable->buckets = (struct Node**)calloc(HASH_SIZE, sizeof(struct Node*));
        if (hashTable->buckets == NULL)
        {
            printf("Cannot allocate memory for buckets\n");
            exit(-1);
        }
        for (int i = 0; i < recordSz; ++i)
        {
            insertRecord(hashTable, pRecords[i]);
        }
        displayRecordsInHash(hashTable);
        freeHashTable(hashTable);
    }
    else
    {
        printf("Cannot allocate memory for the hash table.\n");
    }
    free(pRecords);

    return 0;
}