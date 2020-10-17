#include "Tools.h"

int64_t* intdup(const int64_t* src, size_t len) {
    int64_t* p = malloc(len * sizeof(int64_t));
    memcpy(p, src, len * sizeof(int64_t));

    return p;
}

void cpy_list(LinkedList *dest, LinkedList *src) {
    Node *curr = src->head;

    while(curr != NULL) {
        add_nth_node(dest, get_size(dest), curr->info);
        curr = curr->next;
    }
}

void cpy_paper_lite(Paper_lite* dest, Paper* src) {
    dest->id = src->id;
    dest->num_citations = src->num_citations;
    dest->num_refs = src->num_refs;
    dest->year = src->year;
    dest->references = intdup(src->references, src->num_refs);
    dest->title = strdup(src->title);
}

void free_ht_data() { return; }

void free_ht_list(void* data) {
    LinkedList* tmp = (LinkedList*)data;
    free_list(&tmp);
}

void free_paper_lite(void* data) {
    free(((Paper_lite*)data)->references);
    free(((Paper_lite*)data)->title);
    free(data);
}

void free_paper(void* data) {
    Paper* tmp = (Paper*)data;

    for (int i = 0; i < tmp->num_authors; ++i) {
        free(tmp->institutions[i]);
        free(tmp->authors_names[i]);
    }

    for (int i = 0; i < tmp->num_fields; ++i) {
        free(tmp->fields[i]);
    }

    free_list(&tmp->citations);
    free(tmp->fields);
    free(tmp->institutions);
    free(tmp->authors_names);
    free(tmp->authors_ids);
    free(tmp->references);
    free(tmp->title);
    free(tmp->venue);

    free(tmp);
}

void update_paper(Paper* new, const char* title, const char* venue,
                  const int year, const char** author_names,
                  const int64_t* author_ids, const char** institutions,
                  const int num_authors, const char** fields,
                  const int num_fields, const int64_t* references,
                  const int num_refs) {
    free(new->title);
    new->title = strdup(title);
    DIE(new->title == NULL, "Paper title not initialised.\n");
    free(new->references);
    new->references = intdup(references, num_refs);
    DIE(new->references == NULL, "Paper references not initialised.\n");
    free(new->authors_ids);
    new->authors_ids = intdup(author_ids, num_authors);
    DIE(new->authors_ids == NULL, "Paper authors' IDs not initialised.\n");
    free(new->venue);
    new->venue = strdup(venue);
    DIE(new->venue == NULL, "Paper venue not initialised.\n");
    new->year = year;
    new->num_authors = num_authors;
    new->num_fields = num_fields;
    new->num_refs = num_refs;

    new->institutions = realloc(new->institutions, num_authors * sizeof(char*));
    DIE(new->institutions == NULL,
        "Paper institutions vector not initialised.\n");
    new->authors_names =
        realloc(new->authors_names, num_authors * sizeof(char*));
    DIE(new->authors_names == NULL,
        "Paper authors' names vector not initialised.\n");
    for (int i = 0; i < num_authors; ++i) {
        new->institutions[i] = strdup(institutions[i]);
        DIE(new->institutions[i] == NULL,
            "Paper institution not initialised.\n");
        new->authors_names[i] = strdup(author_names[i]);
        DIE(new->authors_names[i] == NULL,
            "Paper author name not initialised.\n");
    }

    new->fields = realloc(new->fields, num_fields * sizeof(char*));
    DIE(new->fields == NULL, "Paper fields vector not initialised.\n");
    for (int i = 0; i < num_fields; ++i) {
        new->fields[i] = strdup(fields[i]);
        DIE(new->fields[i] == NULL, "Paper field not initialised.\n");
    }
}

Paper* init_empty_paper(const int64_t id) {
    Paper* new;

    new = malloc(sizeof(Paper));
    DIE(new == NULL, "Paper not initialised.\n");

    new->id = id;
    new->citations = malloc(sizeof(LinkedList));
    DIE(new->citations == NULL, "Paper citations vector not initialised.\n");
    init_list(new->citations);
    new->fields = calloc(1, sizeof(char*));
    new->authors_names = calloc(1, sizeof(char*));
    new->institutions = calloc(1, sizeof(char*));
    new->title = calloc(1, sizeof(char*));
    new->venue = calloc(1, sizeof(char*));
    new->authors_ids = calloc(1, sizeof(int64_t));
    new->references = calloc(1, sizeof(int64_t));
    new->num_authors = 0;
    new->num_fields = 0;
    new->num_refs = 0;
    new->year = 0;

    return new;
}

Paper* init_paper(const char* title, const char* venue, const int year,
                  const char** author_names, const int64_t* author_ids,
                  const char** institutions, const int num_authors,
                  const char** fields, const int num_fields, const int64_t id,
                  const int64_t* references, const int num_refs) {
    Paper* new;

    new = malloc(sizeof(Paper));
    DIE(new == NULL, "Paper not initialised.\n");

    new->title = strdup(title);
    DIE(new->title == NULL, "Paper title not initialised.\n");
    new->venue = strdup(venue);
    DIE(new->venue == NULL, "Paper venue not initialised.\n");
    new->references = intdup(references, num_refs);
    DIE(new->references == NULL, "Paper references not initialised.\n");

    new->authors_ids = intdup(author_ids, num_authors);
    DIE(new->authors_ids == NULL, "Paper authors' IDs not initialised.\n");
    new->id = id;
    new->year = year;
    new->num_authors = num_authors;
    new->num_fields = num_fields;
    new->num_refs = num_refs;

    new->citations = malloc(sizeof(LinkedList));
    init_list(new->citations);
    new->num_citations = 0;

    new->fields = malloc(num_fields * sizeof(char*));
    DIE(new->fields == NULL, "Paper fields vector not initialised.\n");
    for (int i = 0; i < num_fields; ++i) {
        new->fields[i] = strdup(fields[i]);
        DIE(new->fields[i] == NULL, "Paper field not initialised.\n");
    }

    new->institutions = malloc(num_authors * sizeof(char*));
    DIE(new->institutions == NULL,
        "Paper institutions vector not initialised.\n");
    new->authors_names = malloc(num_authors * sizeof(char*));
    DIE(new->authors_names == NULL,
        "Paper authors' names vector not initialised.\n");
    for (int i = 0; i < num_authors; ++i) {
        new->institutions[i] = strdup(institutions[i]);
        DIE(new->institutions[i] == NULL,
            "Paper institution not initialised.\n");
        new->authors_names[i] = strdup(author_names[i]);
        DIE(new->authors_names[i] == NULL,
            "Paper author name not initialised.\n");
    }

    return new;
}

void print_paper(Paper* new) {
    Node* curr;

    printf("title: %s\n", new->title);
    printf("id: %ld\n", new->id);
    printf("venue: %s\n", new->venue);
    printf("%d\n", new->year);

    /* for (int i = 0; i < new->num_refs; ++i) {
        printf("ref %d: %ld\n", i, new->references[i]);
    } */

    printf("num_authors: %d\n", new->num_authors);
    for(int i = 0; i < new->num_authors; ++i) {
        printf("auth: %ld\n", new->authors_ids[i]);
    }
    /* printf("num_fields: %d\n", new->num_fields);
    for(int i = 0; i < new->num_fields; ++i) {
        printf("field: %s\n", new->fields[i]);
    } */
    
    printf("num_cit: %d\n", new->num_citations);
    curr = new->citations->head;
    while (curr != NULL) {
        printf("%ld ", *((int64_t*)curr->info));
        curr = curr->next;
    }
    printf("\n\n");
}
