#ifndef PUBLICATIONS_H_
#define PUBLICATIONS_H_

#include <stddef.h>
#include <stdlib.h>

#include "data_structures/Algorithms.h"

/**
 * Will contain all the data that is necessary in order to implement the API
 * given below.
 * Define its fields in publications.c or any other file so that its fields
 * are transparent from outside the API.
 */
struct publications_data;
typedef struct publications_data PublData;

/**
 * Initialises all the fields contained in the PublData structure.
 * Also allocates memory for the structure itself.
 *
 * @return  a pointer to the newly created PublData structure
 */
PublData* init_publ_data(void);

/**
 * Deallocates the memory that was previously allocated for the structure, then
 * deallocates the structure itself.
 *
 * @param data  PublData structure whose data is to be deallocated
 */
void destroy_publ_data(PublData* data);

/**
 * Adds a new paper to the collection of known publications.
 *
 * @param data          the data structure implemented by you
 * @param title         the title of the paper
 * @param year          the year the paper was published
 * @param author_names  the names of the authors
 * @param author_ids    the ids of the authors
 * @param institutions  the names of the author's insititutions
 * @param num_authors   the number of authors and insititutions
 * @param fields        the fields of study that the paper belongs to
 * @param id            a unique integer id of the paper
 * @param references    an array of integers representing the ids of the
 *                      papers the current one references
 * @param num_refs      the length of the references array
 */
void add_paper(PublData* data, const char* title, const char* venue,
               const int year, const char** author_names,
               const int64_t* author_ids, const char** institutions,
               const int num_authors, const char** fields, const int num_fields,
               const int64_t id, const int64_t* references, const int num_refs);

/**
 * Computes the title of the oldest paper that has influenced the one with the
 * given id.
 *
 * @param data      the data structure implemented by you
 * @param id_paper  the id of the paper the query is performed on
 * @return          the name of the oldest influence of the given paper
 */
char* get_oldest_influence(PublData* data, const int64_t id_paper);

/**
 * Calculates the impact factor of the given venue.
 * The impact factor is defined as the average number of citations per paper
 * published in the venue.
 *
 * @param data      the data structure implemented by you
 * @param venue     the name of the venue the query is performed on
 * @return          the desired impact factor
 */
float get_venue_impact_factor(PublData* data, const char* venue);

/**
 * Calculates the number of papers that the given paper has influenced, up to
 * a certain distance.
 *
 * @param data          the data structure implemented by you
 * @param id_paper      the id of the paper the query is performed on
 * @param distance      the maximum distance of influence
 * @return              number of papers that have been influenced by the
 *                      one given as parameter
 */
int get_number_of_influenced_papers(PublData* data, const int64_t id_paper,
                                    const int distance);

/**
 * Calculates the Erdős distance between two authors.
 *
 * @param data  the data structure implemented by you
 * @param id1   the id of one of the authors
 * @param id2   the id of the other author
 * @return      the Erdős distance between the given authors
 */
int get_erdos_distance(PublData* data, const int64_t id1, const int64_t id2);

/**
 * Finds the most cited papers under a certain field of study and places their
 * names in a sorted array.
 * The array should be sorted increasingly by the number of citations.
 *
 * @param data          the data structure implemented by you
 * @param field         the requested field
 * @param num_papers    the requested number of papers; num_papers <= 50
 * @return              an array containing the names of the most cited papers
 *                      sorted increasingly by the number of citations and
 *                      lexicographically for equal number of citations
 */
char** get_most_cited_papers_by_field(PublData* data, const char* field,
                                      int* num_papers);

/**
 * Returns the number of papers that have been published between two given
 * dates.
 * The dates are given as years A.D.
 *
 * @param data          the data structure implemented by you
 * @param early_date    the earlier date
 * @param late_date     the later date; early_date <= late_date
 * @return              the number of papers published between the given dates
 */
int get_number_of_papers_between_dates(PublData* data, const int early_date,
                                       const int late_date);

/**
 * Computes the number of authors that have published papers studying the given
 * field  while working at the given institution.
 *
 * @param data          the data structure implemented by you
 * @param institution   the institution of interest
 * @param field         the field under which the authors have published
 * @return              the number of papers satisfying the above conditions
 */
int get_number_of_authors_with_field(PublData* data, const char* institution,
                                     const char* field);

/**
 * Creates a vector representing the heights of a histogram that contains all
 * citations of a given author.
 * Each bucket of the histogram represents a year (in decreasing order) since
 * the author has started publishing papers and up to the present.
 *
 * @param data          the data structure implemented by you
 * @param id_author     the id of author whose histogram is calculated
 * @param num_years     the number of years since the author's first paper
 * @return              an array containing the histogram values for each year
 */
int* get_histogram_of_citations(PublData* data, const int64_t id_author,
                                int* num_years);

/**
 * Compiles a list of all papers that have influenced a given one, sorted
 * according to the order in which they ought to be read in order to fully
 * understand the given paper.
 * The distance between this paper and any paper in the list should be no more
 * than the specified distance.
 *
 * @param data          the data structure implemented by you
 * @param id_paper      the id of paper that has to be read
 * @param distance      the maximum distance between the given paper and any
 *                      paper in the array
 * @param num_papers    the function writes the number of returned papers in
 *                      this variable
 * @return              a sorted list of papers which if read in the given
 *                      order would result in the best understanding of the
 *                      given paper
 */
char** get_reading_order(PublData* data, const int64_t id_paper,
                         const int distance, int* num_papers);

/**
 * Returns the name of the author that would be the best fit to coordinate a
 * PhD for the given student.
 * The best author is the one that maximizes the score defined by the following
 * formula:
 * score_i = e^{-erdos(author, author_i)} *
 * \sum_{j=1}^{num_papers_author_i}{num_citations_paper_j * impact_factor_j}
 *
 * @param data          the data structure implemented by you
 * @param id_author     the id of the student looking for a coordinator
 * @return              the name of the coordinator
 */
char* find_best_coordinator(PublData* data, const int64_t id_author);

#endif /* PUBLICATIONS_H_ */
