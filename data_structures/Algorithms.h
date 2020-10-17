#ifndef __ALGORITHMS_H
#define __ALGORITHMS_H

#include "Tools.h"

Paper *bfs_refs(HashTable *ht, HashTable *visited, Queue *q_bfs, int64_t id);

int bfs_cit(HashTable *ht, HashTable *visited, Queue *q_bfs, int64_t id,
            int dist);

int bfs_authors(HashTable *ht, HashTable *visited, Queue *q_bfs,
                const int64_t src, const int64_t dest);

LinkedList *bfs_oldest(HashTable *ht, HashTable *visited, Queue *q_bfs,
                       int64_t id, int distance);

Node *split(Node *head);

Node *merge_oldest(Node *first, Node *second);

Node *merge_cit(Node *first, Node *second);

Node *mergeSort(Node *head, Node *(*merge)(Node *, Node *));

#endif