#define _CRT_SECURE_NO_WARNINGS // Behovs for vissa funktioner i visual studio
#include "HashTable.h"
#include "Bucket.h"
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>

// Anvands for att markera en ledig plats i Hashtabellen


/* Denna funktion tar en nyckel och returnerar ett hash-index
dvs ett index till arrayen som Šr Hashtabellen */
static int hash(Key key, int tablesize)
{
    return key % tablesize;
}

//TODO: REMOVE
static void printTable(const HashTable* htable) {
    if(htable->table == NULL) return;
    printf_s("\n#====== Printing table.. ======#\n");
    for(int i = 0; i < htable->size; i++) {
        printf_s("\nhtable->table[%d].key = (%d)", i, htable->table[i].key);
        printf_s("\nhash of key = (%d)", hash(htable->table[i].key, htable->size));
        printf_s("\nindex = (%d)\n", i);
    }
}

/*Leta framŒt enligt principen šppen adressering
 Antalet krockar returneras via pekaren col i parameterlistan*/
static int linearProbe(const HashTable* htable, Key key, unsigned int *col)
{
    int iteration = hash(key, htable->size), end = iteration;
    do {
        printf_s("\nlinearProbe(); @ Key: (%d), Collision amount: (%d), Hash of key/End index: (%d), Iteration index: (%d), htable->size (%d)\n", key, (*col), end, iteration, htable->size);
        if(htable->table[iteration].key != UNUSED) (*col)++;
        else return iteration;
        if(iteration == htable->size-1) iteration = -1;
        iteration++;
    }while(iteration != end);
    return -1; //TODO: Vad ska returneras?
}

/*Allokera minne fšr hashtabellen*/
HashTable createHashTable(unsigned int size)
{
    HashTable hashTable;
    struct Bucket * bucketArray = (struct Bucket *) malloc(sizeof(struct Bucket)*size);
    if(bucketArray != NULL) {
        hashTable.table = bucketArray;
        hashTable.size = size;
        for (int i = 0; i <= size; i++) hashTable.table[i].key = UNUSED; //TODO: REMOVE
        return hashTable;
    }
    hashTable.table = NULL;
    hashTable.size = 0;
    return hashTable;
}

/* Satter in paret {key,data} i Hashtabellen, om en nyckel redan finns ska vardet uppdateras */
/* Returnerar antalet krockar (som rŠknas i linearProbe() )*/
unsigned int insertElement(HashTable* htable, const Key key, const Value value)
{
    if(htable->table == NULL) return 0;
    unsigned int col = 0, *pCol = &col;
    int val = hash(key, htable->size);
    if(htable->table[val].key != key) val = linearProbe(htable, key, pCol);
    htable->table[val].key = key;
    htable->table[val].value = value;
    printf_s("\n#----- @ insertElement(%d); -----#\n", key);
    printTable(htable);
    assert(lookup(htable, key) != NULL);
    return col;
}

/* Tar bort datat med nyckel "key" */
void deleteElement(HashTable* htable, const Key key)
{
    if(htable->table == NULL) return;
    for(int iteration = 0; iteration < htable->size; iteration++)
        if(htable->table[iteration].key == key)
            htable->table[iteration].key = UNUSED;
    printf_s("\n#----- @ deleteElement(%d); -----#\n", key);
    printTable(htable);
    assert(lookup(htable, key) == NULL);
}

const Value* lookup(const HashTable* htable, const Key key)
{
    for(int iteration = 0; iteration < htable->size; iteration++)
        if(htable->table[iteration].key == key)
            return &(htable->table[iteration].value);
    return NULL;
}

/* Returnerar en pekare till vardet som key ar associerat med eller NULL om ingen sadan nyckel finns */
/*const Value* lookup(const HashTable* htable, const Key key)
{
    // Lookup måste titta om key verkligen är key (inte hash comparison)
    int value = hash(key, htable->size);
    if(htable->table[value].key != UNUSED) return &(htable->table[value].value);
    else return NULL;
}*/

/* Tommer Hashtabellen */
void freeHashTable(HashTable* htable)
{
    if(htable->table != NULL) free((*htable).table);
    htable->size = 0;
    assert(htable->table == NULL && htable->size == 0);
}

/* Ger storleken av Hashtabellen */
unsigned int getSize(const HashTable* htable)
{
    return htable->size;
}

/* Denna for att ni enkelt ska kunna visualisera en Hashtabell */
void printHashTable(const HashTable* htable)
{
    if(htable->table == NULL) return;
    for(int i = 0; i < htable->size; i++)
        printPerson(&(htable->table[i]).value, 0);
}

