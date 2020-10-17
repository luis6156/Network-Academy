#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include "Queue.h"

typedef struct info {
    void *key;   /* Data used by the hash */
    void *value; /* Value to be inserted in the HashTable */
} info;

typedef struct HashTable {
    struct LinkedList *buckets; /* Array de liste simplu-inlantuite. */
    int size; /* Nr. total de noduri existente curent in toate bucket-urile. */
    int hmax; /* Nr. de bucket-uri. */
    /* (Pointer la) Functie pentru a calcula valoarea hash asociata cheilor. */
    unsigned int (*hash_function)(void *);
    /* (Pointer la) Functie pentru a compara doua chei. */
    int (*compare_function)(void *, void *);
    /* (Pointer to) Function to delete key */
    void (*free_function)(void *);
} HashTable;

int compare_function_ints(void *a, void *b);

int compare_function_strings(void *a, void *b);

unsigned int hash_function_int(void *a);

unsigned int hash_function_string(void *a);

HashTable *init_ht(int hmax, unsigned int (*hash_function)(void *),
                   int (*compare_function)(void *, void *),
                   void (*free_function)(void *));

void put(HashTable *ht, void *key, int key_size_bytes, void *value);

void *get(HashTable *ht, void *key);

int has_key(HashTable *ht, void *key);

void remove_ht_entry(HashTable *ht, void *key);

void free_ht(HashTable *ht);

#endif