// Copyright 2020 Micu Florian-Luis, Fatu Maria-Miruna, Tema 3 SD, 311CA

#include "Queue.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Initializare structura de tip "coada"
Queue *init_q() {
    Queue *q;

    q = malloc(sizeof(Queue));
    DIE(q == NULL, "Error: queue not initialised.\n");

    q->list = malloc(sizeof(LinkedList));
    DIE(q->list == NULL, "Error: queue list not initialised.\n");

    init_list(q->list);

    return q;
}

// Returneaza marimea unei "cozi"
int get_size_q(Queue *q) { return q->list->size; }

// Verifica daca o "coada" este goala
int is_empty_q(Queue *q) { return get_size_q(q) == 0; }

// Returneaza elementul din varful "cozii"
void *front(Queue *q) {
    if (q == NULL || q->list == NULL) {
        return NULL;
    }

    return q->list->head->info;
}

// Scoate primul element adaugat in "coada"
void dequeue(Queue *q) {
    struct Node *node;
    if (q == NULL || q->list == NULL) {
        return;
    }

    node = remove_nth_node(q->list, 0);
    free(node);
}

// Adauga un element in "coada"
void enqueue(Queue *q, void *new_info) {
    add_nth_node(q->list, q->list->size, new_info);
}

// Sterge elementele din "coada"
void clear_q(Queue *q) {
    struct Node *node;
    while (!is_empty_q(q)) {
        node = remove_nth_node(q->list, 0);
        free(node);
    }
}

// Elibereaza "coada"
void purge_q(Queue *q) {
    clear_q(q);
    free(q->list);
}
