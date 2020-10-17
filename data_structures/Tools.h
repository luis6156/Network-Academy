#ifndef TOOLS_H__
#define TOOLS_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HashTable.h"

#define YEARS 3000

typedef struct Paper_lite {
    int64_t id;
    int64_t *references;
    int year;
    int num_citations;
    int num_refs;
    char *title;
} Paper_lite;

typedef struct Paper {
    int num_authors;
    int num_fields;
    int num_refs;
    int num_citations;
    int year;
    int64_t id;
    int64_t *authors_ids;
    int64_t *references;
    char *title;
    char *venue;
    char **authors_names;
    char **institutions;
    char **fields;
    LinkedList *citations;
} Paper;

int64_t *intdup(const int64_t *src, size_t len);

void cpy_paper_lite(Paper_lite *dest, Paper *src);

void cpy_list(LinkedList *dest, LinkedList *src);

void free_ht_data();

void free_ht_list(void *data);

void free_paper_lite(void *data);

void free_paper(void *data);

void update_paper(Paper *new, const char *title, const char *venue,
                  const int year, const char **author_names,
                  const int64_t *author_ids, const char **institutions,
                  const int num_authors, const char **fields,
                  const int num_fields, const int64_t *references,
                  const int num_refs);

Paper *init_empty_paper(const int64_t id);

void print_paper(Paper *new);

Paper *init_paper(const char *title, const char *venue, const int year,
                  const char **author_names, const int64_t *author_ids,
                  const char **institutions, const int num_authors,
                  const char **fields, const int num_fields, const int64_t id,
                  const int64_t *references, const int num_refs);

#endif