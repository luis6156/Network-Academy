// Copyright 2020 Micu Florian-Luis, Fatu Maria-Miruna, Tema 3 SD, 311CA

#include "LinkedList.h"

#include <stdio.h>
#include <stdlib.h>

// Initializare lista
void init_list(LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Sterge un nod din lista
Node *remove_nth_node(LinkedList *list, int n) {
    Node *prev, *curr;
    int index = 0;

    if (list == NULL || list->head == NULL) {
        return NULL;
    }

    if (n < 0) {
        return NULL;
    }

    if (list->size == 1) {
        curr = list->head;
        list->head->next = NULL;
        list->head->prev = NULL;
        list->head = NULL;
        list->size--;
        return curr;
    } else if (n > list->size - 1) {
        curr = list->tail;
        curr->prev->next = NULL;
        list->head->prev = NULL;
        list->tail = curr->prev;
        curr->prev = NULL;
        curr->next = NULL;
        list->size--;
        return curr;
    } else if (n == 0) {
        curr = list->head;
        curr->next->prev = NULL;
        list->head = curr->next;
        curr->prev = NULL;
        curr->next = NULL;
        list->size--;
        return curr;
    } else {
        curr = list->head->next;
        while (index < n - 1) {
            curr = curr->next;
            index++;
        }
        prev = curr->prev;
        prev->next = curr->next;
        curr->next->prev = prev;
        curr->next = NULL;
        curr->prev = NULL;
        list->size--;
        return curr;
    }
}

// Adaugare in lista pe pozitia potrivita (n incepe de la "0")
void add_nth_node(LinkedList *list, int n, void *new_info) {
    Node *tmp;

    DIE(list == NULL || n < 0, "Error: list uninitialised or n < 0.\n");

    tmp = malloc(sizeof(Node));
    DIE(tmp == NULL, "Error: new node uninitialised.\n");
    tmp->info = new_info;
    tmp->next = NULL;
    tmp->prev = NULL;

    // Daca lista e goala, head-ul si tail-ul listei vor fi noul nod
    if (!list->head || n == 0) {
        list->head = tmp;
        list->tail = tmp;
        tmp->next = NULL;
        tmp->prev = NULL;
    } else if (n + 1 >= list->size) {
        // Daca n + 1 >= marimea listei, se adauga la finalul ei
        tmp->prev = list->tail;
        tmp->next = NULL;
        list->tail->next = tmp;
        list->tail = tmp;
    } else {
        // In ultimul caz, se introduce pe pozitia n (0 < n < list->size - 1)
        Node *it = list->head;
        int k = 0;

        while (k != n) {
            it = it->next;
            k++;
        }

        tmp->next = it->next;
        it->next = tmp;
        tmp->prev = it;
    }
    list->size++;
}

// Returneaza numarul de noduri din lista
int get_size(LinkedList *list) { return list->size; }

// Elibereaza toate elementele din lista, apoi elibereaza lista
void free_list(LinkedList **pp_list) {
    Node *it, *tmp;
    LinkedList *list = *pp_list;

    it = list->head;

    // Elibereaza toate nodurile
    while (it != NULL) {
        tmp = it;
        it = it->next;
        free(tmp);
    }

    // Elibereaza lista
    free(list);
}
