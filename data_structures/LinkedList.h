// Copyright 2020 Micu Florian-Luis, Fatu Maria-Miruna, Tema 3 SD, 311CA

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdio.h>

/**
 * If the assertion is true, the program is closed and the file and line number
 * at which the program closes are printed to `stderr`, together with a given
 * message.
 */
#define DIE(assertion, call_description)                      \
    do {                                                      \
        if (assertion) {                                      \
            fprintf(stderr, "(%s:%u): ", __FILE__, __LINE__); \
            perror(call_description);                         \
            exit(EXIT_FAILURE);                               \
        }                                                     \
    } while (0)

// Structura nod
typedef struct Node {
    void *info;  // Pentru ca datele stocate sa poata avea orice tip, folosim un
                 // pointer la void
    struct Node *next;  // Lista simplu inlantuita
    struct Node *prev;
} Node;

// Strucutra de tip lista simplu inlantuita
typedef struct LinkedList {
    Node *head;
    Node *tail;
    int size;
} LinkedList;

void init_list(LinkedList *list);

Node *remove_nth_node(LinkedList *list, int n);

void add_nth_node(LinkedList *list, int n, void *new_data);

int get_size(LinkedList *list);

void free_list(LinkedList **pp_list);

#endif  // LINKEDLIST_H_
