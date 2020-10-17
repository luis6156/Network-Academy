#include "Algorithms.h"

Paper *bfs_refs(HashTable *ht, HashTable *visited, Queue *q_bfs, int64_t id) {
    Paper *oldest_paper = NULL;
    int64_t cpy_id = id;

    Paper *root = (Paper *)get(ht, &cpy_id);
    Paper_lite *root_tmp = malloc(sizeof(Paper_lite));
    cpy_paper_lite(root_tmp, root);

    put(visited, &root_tmp->id, sizeof(int64_t), root_tmp);

    enqueue(q_bfs, root);

    while (!is_empty_q(q_bfs)) {
        Paper *tmp = (Paper *)front(q_bfs);

        for (int i = 0; i < tmp->num_refs; ++i) {
            if (has_key(ht, &tmp->references[i]) &&
                !has_key(visited, &tmp->references[i])) {
                Paper *new = (Paper *)get(ht, &tmp->references[i]);
                if (new->year != 0) {
                    Paper_lite *new_lite = malloc(sizeof(Paper_lite));
                    cpy_paper_lite(new_lite, new);
                    put(visited, &tmp->references[i], sizeof(int64_t),
                        new_lite);
                    enqueue(q_bfs, new);
                }
            }
        }

        if (oldest_paper == NULL && strcmp(root->title, tmp->title)) {
            oldest_paper = tmp;
        }

        if (oldest_paper != NULL) {
            if (tmp->year < oldest_paper->year) {
                oldest_paper = tmp;
            } else if (tmp->year == oldest_paper->year) {
                if (tmp->num_citations > oldest_paper->num_citations) {
                    oldest_paper = tmp;
                } else if (tmp->num_citations == oldest_paper->num_citations) {
                    if (tmp->id < oldest_paper->id) {
                        oldest_paper = tmp;
                    }
                }
            }
        }
        dequeue(q_bfs);
    }

    if (oldest_paper == NULL) {
        return NULL;
    } else {
        return oldest_paper;
    }
}

int bfs_cit(HashTable *ht, HashTable *visited, Queue *q_bfs, int64_t id,
            int dist) {
    Node *curr;
    int local_dist = 0, impact = 0;

    Paper *root = (Paper *)get(ht, &id);

    if (dist == 1) {
        return root->num_citations;
    }

    Paper_lite *root_tmp = malloc(sizeof(Paper_lite));
    cpy_paper_lite(root_tmp, root);

    put(visited, &root_tmp->id, sizeof(int64_t), root_tmp);

    enqueue(q_bfs, root);
    enqueue(q_bfs, NULL);

    while (!is_empty_q(q_bfs)) {
        if (front(q_bfs) == NULL) {
            local_dist++;
            enqueue(q_bfs, NULL);
            dequeue(q_bfs);
        }

        if (front(q_bfs) == NULL) {
            break;
        }

        if (local_dist >= dist) {
            break;
        }

        Paper *tmp = (Paper *)front(q_bfs);
        dequeue(q_bfs);

        curr = tmp->citations->head;
        while (curr != NULL) {
            int64_t tmp_id = *(int64_t *)curr->info;

            if (has_key(ht, &tmp_id) && !has_key(visited, &tmp_id)) {
                Paper *new = (Paper *)get(ht, &tmp_id);
                if (new->year != 0) {
                    Paper_lite *new_lite = malloc(sizeof(Paper_lite));
                    cpy_paper_lite(new_lite, new);
                    put(visited, &tmp_id, sizeof(int64_t), new_lite);
                    enqueue(q_bfs, new);
                    impact++;
                }
            }
            curr = curr->next;
        }
    }

    return impact;
}

int bfs_authors(HashTable *ht, HashTable *visited, Queue *q_bfs,
                const int64_t src, const int64_t dest) {
    LinkedList *auth_data, *tmp_data;
    Node *curr;
    Paper *paper_tmp;
    int64_t src_cpy = src, id_tmp;
    int dist = 0;

    auth_data = (LinkedList *)get(ht, &src_cpy);

    if (auth_data == NULL) {
        return -1;
    }

    enqueue(q_bfs, auth_data);
    enqueue(q_bfs, NULL);
    put(visited, &src_cpy, sizeof(src_cpy), &src_cpy);

    while (!is_empty_q(q_bfs)) {
        if (front(q_bfs) == NULL) {
            dist++;
            enqueue(q_bfs, NULL);
            dequeue(q_bfs);
        }

        if (front(q_bfs) == NULL) {
            break;
        }

        auth_data = (LinkedList *)front(q_bfs);
        dequeue(q_bfs);

        curr = auth_data->head;
        while (curr != NULL) {
            paper_tmp = (Paper *)curr->info;
            for (int i = 0; i < paper_tmp->num_authors; ++i) {
                id_tmp = paper_tmp->authors_ids[i];
                if (id_tmp == dest) {
                    return dist + 1;
                }
                tmp_data = (LinkedList *)get(ht, &id_tmp);
                if (tmp_data != NULL && !has_key(visited, &id_tmp)) {
                    put(visited, &id_tmp, sizeof(id_tmp), &id_tmp);
                    enqueue(q_bfs, tmp_data);
                }
            }
            curr = curr->next;
        }
    }

    return -1;
}

LinkedList *bfs_oldest(HashTable *ht, HashTable *visited, Queue *q_bfs,
                       int64_t id, int distance) {
    LinkedList *list, *list_tmp;
    Node *curr;
    int local_dist = 0;
    Paper *new, *tmp;

    list = malloc(sizeof(LinkedList));
    list_tmp = malloc(sizeof(LinkedList));
    init_list(list);
    init_list(list_tmp);

    new = (Paper *)get(ht, &id);
    put(visited, &new->id, sizeof(new->id), &new->id);
    enqueue(q_bfs, new);
    enqueue(q_bfs, NULL);

    while (!is_empty_q(q_bfs)) {
        if (front(q_bfs) == NULL) {
            local_dist++;
            /* if(list_tmp->head != NULL) {
                if(list_tmp->head->next != NULL) {
                    list_tmp->head = mergeSort(list_tmp->head, merge_oldest);
                    curr = list_tmp->tail;
                    while (curr->next != NULL) {
                        curr = curr->next;
                    }
                    list_tmp->tail = curr;
                }
                cpy_list(list, list_tmp);
                free_list(&list_tmp);
                list_tmp = malloc(sizeof(LinkedList));
                init_list(list_tmp);
            } */            
            dequeue(q_bfs);
            enqueue(q_bfs, NULL);
        }

        if (front(q_bfs) == NULL) {
            break;
        }

        if (local_dist >= distance) {
            break;
        }

        new = (Paper *)front(q_bfs);
        dequeue(q_bfs);

        for (int i = 0; i < new->num_refs; ++i) {
            if (has_key(ht, &new->references[i]) &&
                !has_key(visited, &new->references[i])) {
                tmp = (Paper *)get(ht, &new->references[i]);
                if (tmp->year != 0) {
                    add_nth_node(list, get_size(list), tmp);
                    enqueue(q_bfs, tmp);
                }
                put(visited, &new->references[i], sizeof(new->references[i]),
                    &new->references[i]);
            }
        }
    }
    free_list(&list_tmp);

    return list;
}

Node *split(Node *head) {
    Node *fast = head;
    Node *slow = head;

    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    Node *tmp = slow->next;
    slow->next = NULL;

    return tmp;
}

Node *merge_oldest(Node *first, Node *second) {
    // First list is empty
    if (!first) {
        return second;
    }

    // Second list is empty
    if (!second) {
        return first;
    }

    if (((Paper *)first->info)->year < ((Paper *)second->info)->year) {
        first->next = merge_oldest(first->next, second);
        first->next->prev = first;
        first->prev = NULL;
        return first;
    } else if (((Paper *)first->info)->year > ((Paper *)second->info)->year) {
        second->next = merge_oldest(first, second->next);
        second->next->prev = second;
        second->prev = NULL;
        return second;
    } else {
        if (((Paper *)first->info)->id < ((Paper *)second->info)->id) {
            first->next = merge_oldest(first->next, second);
            first->next->prev = first;
            first->prev = NULL;
            return first;
        } else {
            second->next = merge_oldest(first, second->next);
            second->next->prev = second;
            second->prev = NULL;
            return second;
        }
    }
}

Node *merge_cit(Node *first, Node *second) {
    // First list is empty
    if (!first) {
        return second;
    }

    // Second list is empty
    if (!second) {
        return first;
    }

    if (((Paper *)first->info)->num_citations >
        ((Paper *)second->info)->num_citations) {
        first->next = merge_cit(first->next, second);
        first->next->prev = first;
        first->prev = NULL;
        return first;
    } else if (((Paper *)first->info)->num_citations <
               ((Paper *)second->info)->num_citations) {
        second->next = merge_cit(first, second->next);
        second->next->prev = second;
        second->prev = NULL;
        return second;
    } else {
        if (((Paper *)first->info)->year > ((Paper *)second->info)->year) {
            first->next = merge_cit(first->next, second);
            first->next->prev = first;
            first->prev = NULL;
            return first;
        } else if (((Paper *)first->info)->year <
                   ((Paper *)second->info)->year) {
            second->next = merge_cit(first, second->next);
            second->next->prev = second;
            second->prev = NULL;
            return second;
        } else {
            if (((Paper *)first->info)->id < ((Paper *)second->info)->id) {
                first->next = merge_cit(first->next, second);
                first->next->prev = first;
                first->prev = NULL;
                return first;
            } else {
                second->next = merge_cit(first, second->next);
                second->next->prev = second;
                second->prev = NULL;
                return second;
            }
        }
    }
}

Node *mergeSort(Node *head, Node *(*merge)(Node *, Node *)) {
    if (!head || !head->next) {
        return head;
    }

    // Find the middle of the list
    Node *second = split(head);

    // Recur for left and right halves
    head = mergeSort(head, merge);
    second = mergeSort(second, merge);

    // Merge the two sorted halves
    return merge(head, second);
}
