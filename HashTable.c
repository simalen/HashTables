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

/*Leta framŒt enligt principen šppen adressering
 Antalet krockar returneras via pekaren col i parameterlistan*/
static int linearProbe(const HashTable* htable, Key key, unsigned int *col)
{
    unsigned int iteration = hash(key, htable->size), index = iteration;
    while(*col < htable->size) {
        if(htable->table[index].key == UNUSED || htable->table[index].key == key) return index;
        (*col)++;
        index = (*col + iteration) % htable->size;
    }
    return -1;
}

/*Allokera minne fšr hashtabellen*/
HashTable createHashTable(unsigned int size)
{
    HashTable hashTable;
    struct Bucket * bucketArray = calloc(sizeof(*bucketArray), size);
    if(bucketArray != NULL) {
        hashTable.table = bucketArray;
        hashTable.size = size;
    }
    else {
        hashTable.table = NULL;
        hashTable.size = 0;
    }
    return hashTable;
}

/* Satter in paret {key,data} i Hashtabellen, om en nyckel redan finns ska vardet uppdateras */
/* Returnerar antalet krockar (som rŠknas i linearProbe() )*/
unsigned int insertElement(HashTable* htable, const Key key, const Value value)
{
    if(htable->table == NULL) return 0;
    unsigned int col = 0, *pCol = &col;
    int index = linearProbe(htable, key, pCol);
    htable->table[index].key = key;
    htable->table[index].value = value;
    assert(lookup(htable, key) != NULL);
    return col;
}

/* Tar bort datat med nyckel "key" */
void deleteElement(HashTable* htable, const Key key)
{
    unsigned int col = 0, index = linearProbe(htable, key, &col);
    if(htable->table[index].key == UNUSED || index == -1) return;
    htable->table[index].key = UNUSED;
    for(int iteration = 1; iteration < htable->size; iteration++) {
        index = hash(key + iteration + col, htable->size);
        if(htable->table[index].key == UNUSED) break;
        if(index != hash(htable->table[index].key, htable->size)) { // Se om man kan ändra den här
            int T = htable->table[index].key;
            htable->table[index].key = UNUSED;
            insertElement(htable, T, htable->table[index].value);
        }
    }
    assert(lookup(htable, key) == NULL);
}

/* Returnerar en pekare till vardet som key ar associerat med eller NULL om ingen sadan nyckel finns */
const Value* lookup(const HashTable* htable, const Key key)
{
    unsigned int col = 0, index = linearProbe(htable, key, &col);
    if(htable->table[index].key == UNUSED || index == -1) return NULL;
    for(int iteration = 0; iteration < htable->size; iteration++) {
        index = hash(key + iteration + col, htable->size);
        if(htable->table[index].key == key) return &(htable->table[index].value);
    }
    return NULL;
}


/* Tommer Hashtabellen */
void freeHashTable(HashTable* htable)
{
    free((*htable).table);
    htable->size = 0;
    (*htable).table = NULL;
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
