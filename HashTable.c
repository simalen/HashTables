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
    int iteration = hash(key, htable->size), coll = 0, end = iteration;
    while(lookup(htable, hash(key, htable->size) != NULL)) {
        if(iteration == htable->size) iteration = 0;
        iteration++;
        if(iteration == end) return -1; //TODO: Vad returnerar den ifall tabellen är full?
    }
    *col = coll;
    return hash(iteration, htable->size);

    /*TODO: Titta ifall det finns konflikt när värden är lika med varandra.
     *      hash(); = index? */
}

/*Allokera minne fšr hashtabellen*/
HashTable createHashTable(unsigned int size)
{
    HashTable hashTable;
    struct Bucket * bucketArray = (struct Bucket *) malloc(sizeof(struct Bucket)*size);
    if(bucketArray != NULL) {
        hashTable.table = bucketArray;
        hashTable.size = size;
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
    unsigned int col, *pCol = &col;
    int hash = linearProbe(htable, key, pCol);
    htable->table[hash].key = key;
    htable->table[hash].value = value;
    assert(lookup(htable, key) != NULL);
    return col;
}

/* Tar bort datat med nyckel "key" */
void deleteElement(HashTable* htable, const Key key)
{
    if(htable->table == NULL) return;
    int val = hash(key, htable->size);
    htable->table[val].key = 0;
    /*TODO: Value behöver inte uppdateras eftersom att det ändå kommer att skrivas över
     *      när det väl sätts in ett nytt värde */

    assert(lookup(htable, key) == NULL);
}

/* Returnerar en pekare till vardet som key ar associerat med eller NULL om ingen sadan nyckel finns */
const Value* lookup(const HashTable* htable, const Key key)
{
    int val = hash(key, htable->size);
    if(htable->table[val].key == key) return &(htable->table[val].value); //TODO: Antar att detta är rätt vis att göra det på.
    else return NULL;
}

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
