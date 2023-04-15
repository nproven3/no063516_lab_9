#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    // create a next pointer (for linkedlist structure)
    struct RecordType *next; 
};

// Fill out this structure
struct HashType
{
    struct RecordType *record; // create a pointer to a RecordType
};

//Prototyped functions just good practice
int hash(int x, int tableSize); 
int parseData(char *inputFileName, struct RecordType **ppData);
void printRecords(struct RecordType pData[], int dataSz);
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize);
void displayRecordsInHash(struct HashType *hashTable, int tableSize);

// Compute the hash function
int hash(int x, int tableSize)
{
    // hash function is x % tableSize
    int hash = x % tableSize;
    return hash;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);

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
            pRecord->next = NULL;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    printf("\n");
    printf("Records:\n");
    for (int i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
    // call the hash function to get the index
    int index = hash(record->id, tableSize);

    // if the RecordType at that index is NULL
    if (hashTable[index].record == NULL)
    {
        // set 'record' equal to the HashType pointer in the table at index
        hashTable[index].record = record;
    }
    else
    {
        // else traverse to the end of the linkedlist and add 'record' to the end of it
        struct RecordType *current = hashTable[index].record;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = record;
    }
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable, int tableSize) //Takes in the address and the number of indecies in the table
{
    int i;
    printf("\nHash Records:\n");

    for (i = 0; i < tableSize; ++i) //Goes for the number of elements in the table
    {
        printf("\tIndex %d -> ", i);
        struct RecordType *currentRecord = hashTable[i].record;

        if (currentRecord == NULL) //Checks to see if an index is empty if it is just prints "NULL"
        {
            printf("NULL"); //Moves onto next indecie immitdiately after printing NULL to mark as empty
        }
        else
        {
            printf("%d, %c, %d", currentRecord -> id, currentRecord -> name, currentRecord -> order);
            while (currentRecord -> next != NULL) 
            {
                currentRecord = currentRecord -> next;
                printf(" -> %d, %c, %d", currentRecord -> id, currentRecord -> name, currentRecord -> order);
                //The print statement above took a while to get just right.
            }
            
            printf(" -> NULL");//Just shows that at the end of the chain there is a null statement

        }
        printf("\n"); //Moves to the next line each iteration for the for loop sor it prints properly
    }
    printf("\n");
}


int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    
    int size = 11; //Determines the size of the table
    
    struct HashType *hash = (struct HashType *)calloc(size, sizeof(struct HashType));//Instructor told me to use calloc instead of malloc
    //I had never used calloc before this and had no idea how to do it. So I had to figure this out on my own.
    for (int i = 0; i < recordSz; i++) {
        insertRecord(hash, &pRecords[i], size);
    }

    //Displays all of the hash table
    displayRecordsInHash(hash, size);

    // the two statements are sufficient to free all of the memory a loop is not necessary
    // THIS HAS BEEN TESTED IN VALGRIND NO MEMORY LEAKS OCCUR WITH THE GCC COMPILER
    free(pRecords); 
    free(hash);

    return 0;
}
