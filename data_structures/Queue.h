// Copyright 2020 Micu Florian-Luis, Fatu Maria-Miruna, Tema 3 SD, 311CA

#ifndef QUEUE_H_
#define QUEUE_H_

#include "LinkedList.h"

typedef struct {
    LinkedList *list;
} Queue;

Queue *init_q();

int get_size_q(Queue *q);

int is_empty_q(Queue *q);

void *front(Queue *q);

void dequeue(Queue *q);

void enqueue(Queue *q, void *new_data);

void clear_q(Queue *q);

void purge_q(Queue *q);

#endif  // QUEUE_H_
