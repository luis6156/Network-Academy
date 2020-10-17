#include "./publications.h"

#define YEARS 3000
#define HMAX 10000
#define MAX_VEN 1000
#define MAX_AUTH 3000
#define MAX_FIELD 3000
#define MAX_LITE 500
#define CURR_YEAR 2020

struct publications_data {
    /* TODO: add whichever data you need here */
    HashTable *main;
    HashTable *ht_venue;
    HashTable *ht_authors;
    HashTable *ht_fields;
    int *years_data;
};

PublData *init_publ_data(void) {
    /* TODO: implement init_publ_data */
    PublData *data;

    data = malloc(sizeof(PublData));

    data->main =
        init_ht(HMAX, hash_function_int, compare_function_ints, free_paper);
    data->ht_venue = init_ht(MAX_VEN, hash_function_int, compare_function_ints,
                             free_ht_list);
    data->ht_authors = init_ht(MAX_AUTH, hash_function_int,
                               compare_function_ints, free_ht_list);
    data->ht_fields = init_ht(MAX_FIELD, hash_function_int,
                              compare_function_ints, free_ht_list);

    data->years_data = calloc(YEARS, sizeof(int));

    return data;
}

void destroy_publ_data(PublData *data) {
    /* TODO: implement destroy_publ_data */

    free_ht(data->main);
    free_ht(data->ht_venue);
    free_ht(data->ht_authors);
    free_ht(data->ht_fields);

    free(data->years_data);
    free(data);
}

void add_paper(PublData *data, const char *title, const char *venue,
               const int year, const char **author_names,
               const int64_t *author_ids, const char **institutions,
               const int num_authors, const char **fields, const int num_fields,
               const int64_t id, const int64_t *references,
               const int num_refs) {
    /* TODO: implement add_paper */
    LinkedList *ven_data, *auth_data, *field_data;
    Paper *tmp, *new;
    int64_t id_cpy = id, id_author;
    unsigned int ven_id, field_id;
    char *ven_cpy, *field_cpy;

    // Check if paper exists already
    if (has_key(data->main, &id_cpy)) {
        new = (Paper *)get(data->main, &id_cpy);
        update_paper(new, title, venue, year, author_names, author_ids,
                     institutions, num_authors, fields, num_fields, references,
                     num_refs);
    } else {
        new = init_paper(title, venue, year, author_names, author_ids,
                         institutions, num_authors, fields, num_fields, id,
                         references, num_refs);
        put(data->main, &id_cpy, sizeof(id_cpy), new);
    }

    // Fields HashTable
    for (int i = 0; i < num_fields; ++i) {
        field_cpy = strdup(fields[i]);
        field_id = hash_function_string(field_cpy);
        field_data = (LinkedList *)get(data->ht_fields, &field_id);
        if (field_data != NULL) {
            add_nth_node(field_data, get_size(field_data), new);
        } else {
            field_data = malloc(sizeof(LinkedList));
            init_list(field_data);
            add_nth_node(field_data, get_size(field_data), new);
            put(data->ht_fields, &field_id, sizeof(field_id), field_data);
        }
        free(field_cpy);
    }

    // Authors HashTable
    for (int i = 0; i < num_authors; ++i) {
        id_author = author_ids[i];
        auth_data = (LinkedList *)get(data->ht_authors, &id_author);
        if (auth_data != NULL) {
            add_nth_node(auth_data, get_size(auth_data), new);
        } else {
            auth_data = malloc(sizeof(LinkedList));
            init_list(auth_data);
            add_nth_node(auth_data, get_size(auth_data), new);
            put(data->ht_authors, &id_author, sizeof(id_author), auth_data);
        }
    }

    // Venue HashTable
    ven_cpy = strdup(venue);
    ven_id = hash_function_string(ven_cpy);
    if (has_key(data->ht_venue, &ven_id)) {
        ven_data = (LinkedList *)get(data->ht_venue, &ven_id);
        add_nth_node(ven_data, get_size(ven_data), &new->id);
    } else {
        ven_data = malloc(sizeof(LinkedList));
        init_list(ven_data);
        add_nth_node(ven_data, get_size(ven_data), &new->id);
        put(data->ht_venue, &ven_id, sizeof(ven_id), ven_data);
    }
    free(ven_cpy);

    // Check if every reference exists already
    for (int i = 0; i < num_refs; ++i) {
        int64_t ref_cpy = references[i];
        if (has_key(data->main, &ref_cpy)) {
            tmp = get(data->main, &ref_cpy);
            add_nth_node(tmp->citations, get_size(tmp->citations), &new->id);
            tmp->num_citations++;
        } else {
            tmp = init_empty_paper(ref_cpy);
            add_nth_node(tmp->citations, get_size(tmp->citations), &new->id);
            tmp->num_citations = 1;
            put(data->main, &ref_cpy, sizeof(ref_cpy), tmp);
        }
    }

    data->years_data[year]++;
}

char *get_oldest_influence(PublData *data, const int64_t id_paper) {
    /* TODO: implement get_oldest_influence */
    Paper *result;
    Queue *q_bfs;
    HashTable *visited;
    int64_t id_cpy = id_paper;

    visited = init_ht(MAX_LITE, hash_function_int, compare_function_ints,
                      free_paper_lite);

    q_bfs = init_q();

    result = bfs_refs(data->main, visited, q_bfs, id_cpy);

    purge_q(q_bfs);
    free(q_bfs);
    free_ht(visited);

    if (result == NULL) {
        return "None";
    } else {
        return result->title;
    }
}

float get_venue_impact_factor(PublData *data, const char *venue) {
    /* TODO: implement get_venue_impact_factor */
    LinkedList *tmp;
    Node *curr;
    int64_t id;
    unsigned int ven_id;
    char *ven_cpy;
    int num_citations = 0, num_papers;

    ven_cpy = strdup(venue);
    ven_id = hash_function_string(ven_cpy);
    tmp = (LinkedList *)get(data->ht_venue, &ven_id);
    curr = tmp->head;

    while (curr != NULL) {
        id = *(int64_t *)curr->info;
        if (has_key(data->main, &id)) {
            num_citations += ((Paper *)get(data->main, &id))->num_citations;
        }
        curr = curr->next;
    }

    num_papers = tmp->size;
    free(ven_cpy);

    return (float)num_citations / num_papers;
}

int get_number_of_influenced_papers(PublData *data, const int64_t id_paper,
                                    const int distance) {
    /* TODO: implement get_number_of_influenced_papers */
    Queue *q_bfs;
    HashTable *visited;
    int influence;
    int64_t id_cpy = id_paper;

    visited = init_ht(MAX_LITE, hash_function_int, compare_function_ints,
                      free_paper_lite);

    q_bfs = init_q();

    influence = bfs_cit(data->main, visited, q_bfs, id_cpy, distance);

    purge_q(q_bfs);
    free(q_bfs);
    free_ht(visited);

    return influence;
}

int get_erdos_distance(PublData *data, const int64_t id1, const int64_t id2) {
    /* TODO: implement get_erdos_distance */
    HashTable *visited;
    Queue *q_bfs;
    int dist = 0;

    visited = init_ht(MAX_AUTH, hash_function_int, compare_function_ints,
                      free_ht_data);
    q_bfs = init_q();

    dist = bfs_authors(data->ht_authors, visited, q_bfs, id1, id2);

    purge_q(q_bfs);
    free(q_bfs);
    free_ht(visited);

    return dist;
}

char **get_most_cited_papers_by_field(PublData *data, const char *field,
                                      int *num_papers) {
    /* TODO: implement get_most_cited_papers_by_field */
    LinkedList *field_data;
    Node *curr;
    unsigned int field_id;
    char **result;
    char *field_cpy;

    field_cpy = strdup(field);
    field_id = hash_function_string(field_cpy);
    field_data = (LinkedList *)get(data->ht_fields, &field_id);
    free(field_cpy);

    if (field_data->head == NULL) {
        return NULL;
    }

    field_data->head = mergeSort(field_data->head, merge_cit);
    curr = field_data->tail;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    field_data->tail = curr;

    result = malloc(*num_papers * sizeof(char *));

    curr = field_data->head;
    for (int i = 0; i < *num_papers; ++i) {
        if (curr == NULL) {
            *num_papers = i;
            result = realloc(result, *num_papers * sizeof(char *));
            break;
        }

        result[i] = strdup(((Paper *)curr->info)->title);
        curr = curr->next;
    }

    return result;
}

int get_number_of_papers_between_dates(PublData *data, const int early_date,
                                       const int late_date) {
    /* TODO: implement get_number_of_papers_between_dates */
    int sum = 0;

    for (int i = early_date; i <= late_date; i++) {
        sum += data->years_data[i];
    }

    return sum;
}

int get_number_of_authors_with_field(PublData *data, const char *institution,
                                     const char *field) {
    /* TODO: implement get_number_of_authors_with_field */
    HashTable *visited;
    LinkedList *field_data;
    Paper *tmp;
    Node *curr;
    char *field_cpy;
    int64_t id_cpy;
    unsigned int field_id;
    int sol = 0;

    field_cpy = strdup(field);
    field_id = hash_function_string(field_cpy);
    free(field_cpy);

    field_data = (LinkedList *)get(data->ht_fields, &field_id);

    curr = field_data->head;

    if (curr == NULL) {
        return 0;
    }

    visited = init_ht(MAX_LITE, hash_function_int, compare_function_ints,
                      free_ht_data);

    while (curr != NULL) {
        tmp = (Paper *)curr->info;
        for (int i = 0; i < tmp->num_authors; ++i) {
            if (!strcmp(tmp->institutions[i], institution) &&
                !has_key(visited, &tmp->authors_ids[i])) {
                id_cpy = tmp->authors_ids[i];
                put(visited, &id_cpy, sizeof(id_cpy), &id_cpy);
                sol++;
            }
        }
        curr = curr->next;
    }

    free_ht(visited);

    return sol;
}

int *get_histogram_of_citations(PublData *data, const int64_t id_author,
                                int *num_years) {
    /* TODO: implement get_histogram_of_citations */
    LinkedList *auth_data;
    Paper *tmp;
    Node *curr;
    int64_t id_cpy = id_author;
    int *sol;
    int MIN = 20000;

    *num_years = 0;
    auth_data = (LinkedList *)get(data->ht_authors, &id_cpy);

    if (auth_data == NULL) {
        return NULL;
    }

    sol = calloc(MIN, sizeof(int));

    curr = auth_data->head;
    while (curr != NULL) {
        tmp = (Paper *)curr->info;
        if (tmp->year < MIN) {
            MIN = tmp->year;
        }
        sol[CURR_YEAR - tmp->year] += tmp->num_citations;
        curr = curr->next;
    }

    *num_years = CURR_YEAR - MIN + 1;
    sol = realloc(sol, *num_years * sizeof(int));

    return sol;
}

char **get_reading_order(PublData *data, const int64_t id_paper,
                         const int distance, int *num_papers) {
    /* TODO: implement get_reading_order */
    LinkedList *list;
    Node *curr;
    Queue *q_bfs;
    HashTable *visited;
    int64_t id_cpy = id_paper;
    char **titles;

    visited = init_ht(MAX_LITE, hash_function_int, compare_function_ints,
                      free_ht_data);

    q_bfs = init_q();

    if(id_cpy == 1486828095) {
        printf("ok\n");
    }

    *num_papers = 0;
    list = bfs_oldest(data->main, visited, q_bfs, id_cpy, distance);

    purge_q(q_bfs);
    free(q_bfs);
    free_ht(visited);

    if (list->head == NULL) {
        return NULL;
    }

    *num_papers = list->size;
    
    list->head = mergeSort(list->head, merge_oldest);
    curr = list->tail;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    list->tail = curr;

    titles = malloc(*num_papers * sizeof(char *));
    curr = list->head;
    for (int i = 0; i < *num_papers; ++i) {
        titles[i] = strdup(((Paper *)curr->info)->title);
        curr = curr->next;
    }

    free_list(&list);

    return titles;
}

char *find_best_coordinator(PublData *data, const int64_t id_author) {
    /* TODO: implement find_best_coordinator */

    return NULL;
}
