#include "HashTable.h"

#include <stdlib.h>
#include <string.h>

int compare_function_ints(void *a, void *b) {
    int int_a = *((int *)a);
    int int_b = *((int *)b);

    if (int_a == int_b) {
        return 0;
    } else if (int_a < int_b) {
        return -1;
    } else {
        return 1;
    }
}

int compare_function_strings(void *a, void *b) {
    char *str_a = (char *)a;
    char *str_b = (char *)b;

    return strcmp(str_a, str_b);
}

unsigned int hash_function_int(void *a) {
    /*
     * Credits: https://stackoverflow.com/a/12996028/7883884
     */
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

unsigned int hash_function_string(void *a) {
    /*
     * Credits: http://www.cse.yorku.ca/~oz/hash.html
     */
    unsigned char *puchar_a = (unsigned char *)a;
    unsigned long hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

    return hash;
}

HashTable *init_ht(int hmax, unsigned int (*hash_function)(void *),
                   int (*compare_function)(void *, void *),
                   void (*free_function)(void *)) {
    HashTable *ht = malloc(sizeof(HashTable));

    ht->size = 0;
    ht->hmax = hmax;
    ht->compare_function = compare_function;
    ht->hash_function = hash_function;
    ht->free_function = free_function;

    ht->buckets = malloc(hmax * sizeof(HashTable));
    for (int i = 0; i < hmax; i++) {
        init_list(&ht->buckets[i]);
    }

    return ht;
}

// Direct Chaining
void put(HashTable *ht, void *key, int key_size_bytes, void *value) {
    info *info_tmp;
    int index = ht->hash_function(key) % ht->hmax;

    // A mai fost introdus deja
    if (has_key(ht, key)) {
        return;
    }

    info_tmp = malloc(sizeof(info));
    info_tmp->key = malloc(key_size_bytes);
    memcpy(info_tmp->key, key, key_size_bytes);

    info_tmp->value = value;

    add_nth_node(&ht->buckets[index], get_size(&ht->buckets[index]), info_tmp);
    ht->size++;
}

void *get(HashTable *ht, void *key) {
    int index = ht->hash_function(key) % ht->hmax;
    Node *it = ht->buckets[index].head;

    while (it != NULL) {
        if (ht->compare_function(((info *)it->info)->key, key) == 0) {
            return ((struct info *)it->info)->value;
        }
        it = it->next;
    }

    return NULL;
}

int has_key(HashTable *ht, void *key) {
    int index = ht->hash_function(key) % ht->hmax;
    Node *it = ht->buckets[index].head;

    while (it != NULL) {
        if (ht->compare_function(((info *)it->info)->key, key) == 0) {
            return 1;
        }
        it = it->next;
    }

    return 0;
}

void remove_ht_entry(HashTable *ht, void *key) {
    Node *tmp;
    int index = ht->hash_function(key) % ht->hmax;
    Node *it = ht->buckets[index].head;
    int count = 0;

    while (it != NULL) {
        if (ht->compare_function(((info *)it->info)->key, key) == 0) {
            break;
        }
        count++;
        it = it->next;
    }

    // Didn't find the key
    if (!it) {
        return;
    }

    tmp = remove_nth_node(&ht->buckets[index], count);
    free((((info *)tmp->info)->key));
    free(((info *)tmp->info));
    free(tmp);

    ht->size--;
}

void free_ht(HashTable *ht) {
    Node *tmp;

    for (int i = 0; i < ht->hmax; i++) {
        Node *curr = ht->buckets[i].head;
        while (curr != NULL) {
            tmp = curr;
            curr = curr->next;
            info *data = (info *)tmp->info;
            ht->free_function(data->value);
            free(data->key);
            free(data);
            free(tmp);
        }
    }
    free(ht->buckets);
    free(ht);
}
