#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "runner.h"

#define EPS					(.001f)
#define MAX_BUFFER			(1024)
#define MAX_INPUT_NAME		(256)

/**
 * Tests if two strings are equal.
 * If they are not, the macro prints an error message end returns the error code
 * (-1).
 */
#define SD_TEST_STRINGS(out_str, ref_str, param)						\
	do																	\
	{																	\
		if (!out_str || strcmp(out_str, ref_str))						\
		{																\
			printf("ERR [%s:%d]\ninput = %s\noutput = %s\nref = %s\n",	\
				__func__, __LINE__, param, out_str, ref_str);			\
			return -1;													\
		}																\
	} while(0)

/**
 * Tests if two ints are equal.
 * If they are not, the macro prints an error message end returns the error code
 * (-1).
 */
#define SD_TEST_INTS(out_num, ref_num, param)							\
	do																	\
	{																	\
		if (out_num != ref_num)											\
		{																\
			printf("ERR [%s:%d]\ninput = %s\noutput = %d\nref = %d\n",	\
				__func__, __LINE__, param, out_num, ref_num);			\
			return -1;													\
		}																\
	} while(0)

/**
 * Tests if two floats are equal (within the EPS precision).
 * If they are not, the macro prints an error message end returns the error code
 * (-1).
 */
#define SD_TEST_FLOATS(out_num, ref_num, param)							\
	do																	\
	{																	\
		if (fabs(out_num - ref_num) > EPS)								\
		{																\
			printf("ERR [%s:%d]\ninput = %s\noutput = %f\nref = %f\n",	\
				__func__, __LINE__, param, out_num, ref_num);			\
			return -1;													\
		}																\
	} while(0)

/**
 * Each of the following functions calls its counterpart in the API and checks
 * the result given by the latter, with the exception of run_add_paper, for
 * which, add_paper doesn't return anything. Thus, run_add_paper always returns
 * 0.
 *
 * @param data		the structure containing the information required for
 *					the publications API
 * @param args		a string containing the arguments of the API function
 *					where the arguments are separated between spaces
 * @param ref_file	the file that the output of the query will be written to
 * @return			a code symbolising the correctness of the execution:
 *					* 0: success
 *					* -1: failure
 */

/**
 * Uses parson in order to parse the paper given as input and calls add_paper
 * with its data. At the end, the function frees all memory allocated for the
 * parameters of add_paper!			
 */
static int run_add_paper(PublData* data, const char* args)
{
	char *input_name;
	char* title, *venue, **author_names, **institutions, **fields;
	int i, year, num_authors, num_fields, num_refs;
	int64_t* references, *author_ids, id;
	JSON_Object* author, *obj;
	JSON_Array* fields_of_study, *authors, *refs;
	JSON_Value* paper;

	asprintf(&input_name, "parsed_papers/paper_%s", args);
	DIE(input_name == NULL, "asprintf");

	paper = json_parse_file(input_name);
	DIE(paper == NULL, "json_parse_file");

	obj = json_object(paper);
	DIE(obj == NULL, "json_object");

	id = atoll(json_object_get_string(obj, "id"));

	title = (char *)json_object_get_string(obj, "title");
	DIE(title == NULL, "title");

	venue = (char *)json_object_get_string(obj, "venue");
	DIE(venue == NULL, "venue");

	year = (int)json_object_get_number(obj, "year");

	authors = json_object_get_array(obj, "authors");
	DIE(authors == NULL, "authors");

	num_authors = json_array_get_count(authors);

	institutions = malloc(num_authors * sizeof(*institutions));
	DIE(institutions == NULL, "malloc - institutions");

	author_names = malloc(num_authors * sizeof(*author_names));
	DIE(author_names == NULL, "malloc - author names");

	author_ids = malloc(num_authors * sizeof(*author_ids));
	DIE(author_ids == NULL, "malloc - author ids");

	for (i = 0; i < num_authors; ++i)
	{
		author = json_array_get_object(authors, i);

		author_names[i] = (char *)json_object_get_string(author, "name");
		DIE(author_names[i] == NULL, "strdup - author_names");

		author_ids[i] = atoll(json_object_get_string(author, "id"));

		institutions[i] = (char *)json_object_get_string(author, "org");
		DIE(institutions[i] == NULL, "strdup - institutions");
	}

	fields_of_study = json_object_get_array(obj, "fields_of_study");
	DIE(fields_of_study == NULL, "fields of study");

	num_fields = json_array_get_count(fields_of_study);

	fields = malloc(num_fields * sizeof(*institutions));
	DIE(fields == NULL, "malloc - fields");

	for (i = 0; i < num_fields; ++i)
	{
		fields[i] = (char *)json_array_get_string(fields_of_study, i);
		DIE(fields[i] == NULL, "strdup - fields");
	}

	refs = json_object_get_array(obj, "references");
	DIE(refs == NULL, "references");

	num_refs = json_array_get_count(refs);

	references = malloc(num_refs * sizeof(*references));
	DIE(references == NULL, "malloc - references");

	for (i = 0; i < num_refs; ++i)
	{
		references[i] = atoll(json_array_get_string(refs, i));
	}

	add_paper(data, title, venue, year, (const char**)author_names,
		(const int64_t*)author_ids, (const char**)institutions,	num_authors,
		(const char**)fields, num_fields, id, references, num_refs);

	free(input_name);
	json_object_clear(obj);
	json_value_free(paper);

	/* All data passed to add_paper is freed after the function returns. */
	free(author_names);
	free(institutions);
	free(author_ids);

	free(fields);

	free(references);

	return 0;
}

static int check_oldest_influence(PublData* data, char* args,
	FILE* ref_file)
{
	char ref_oldest_influence[MAX_BUFFER];
	char* oldest_influence = get_oldest_influence(data, atoll(args));

	fgets(ref_oldest_influence, MAX_BUFFER, ref_file);
	ref_oldest_influence[strlen(ref_oldest_influence) - 1] = '\0';

	SD_TEST_STRINGS(oldest_influence, ref_oldest_influence, args);

	return 0;
}

static int check_impact_factor(PublData* data, char* args, FILE* ref_file)
{
	float impact_factor = get_venue_impact_factor(data, args);
	float ref_impact_factor;

	fscanf(ref_file, "%f\n", &ref_impact_factor);

	SD_TEST_FLOATS(impact_factor, ref_impact_factor, args);

	return 0;
}

static int check_influenced_papers(PublData* data, char* args,
	FILE* ref_file)
{
	char params[MAX_BUFFER];
	int64_t id_paper = atoll(args);
	char* p_space;
	int distance;
	int num_influenced;
	int ref_num_influenced;

	strcpy(params, args);

	p_space = strrchr(args, ' ');
	*p_space = '\0';
	distance = atoi(p_space + 1);

	num_influenced = get_number_of_influenced_papers(data, id_paper,
		distance);

	fscanf(ref_file, "%d\n", &ref_num_influenced);
	SD_TEST_INTS(num_influenced, ref_num_influenced, params);

	return 0;
}

static int check_erdos_distance(PublData* data, char* args, FILE* ref_file)
{
	int64_t id1 = atoll(args);
	int64_t id2 = atoll(strchr(args, ' ') + 1);
	int distance = get_erdos_distance(data, id1, id2);
	int ref_distance;

	fscanf(ref_file, "%d\n", &ref_distance);
	SD_TEST_INTS(distance, ref_distance, args);

	return 0;
}

/*
 * Frees the memory allocated for the output of
 * get_most_cited_papers_by_field!
 */
static int check_most_cited_papers(PublData* data, char* args,
	FILE* ref_file)
{
	int i;
	char params[MAX_BUFFER];
	char ref_paper[MAX_BUFFER];
	char* field = args;
	char* p_space;
	int num_papers;
	int ref_num_papers;
	char** papers;

	strcpy(params, args);
	
	p_space = strrchr(args, ' ');
	*p_space = '\0';

	num_papers = atoi(p_space + 1);

	papers = get_most_cited_papers_by_field(data, field, &num_papers);

	fscanf(ref_file, "%d\n", &ref_num_papers);
	SD_TEST_INTS(num_papers, ref_num_papers, params);

	for (i = 0; i < num_papers; ++i)
	{
		fgets(ref_paper, MAX_BUFFER, ref_file);
		ref_paper[strlen(ref_paper) - 1] = '\0';

		SD_TEST_STRINGS(papers[i], ref_paper, params);

		free(papers[i]);
	}
	free(papers);

	return 0;
}

static int check_papers_between_dates(PublData* data, char* args,
	FILE* ref_file)
{
	int early_date = atoi(args);
	int late_date = atoi(strchr(args, ' ') + 1);
	int num_papers = get_number_of_papers_between_dates(data, early_date,
		late_date);
	int ref_num_papers;

	fscanf(ref_file, "%d\n", &ref_num_papers);
	SD_TEST_INTS(num_papers, ref_num_papers, args);

	return 0;
}

static int check_authors_with_field(PublData* data, char* args,
	FILE* ref_file)
{
	char params[MAX_BUFFER];
	char* institution = args;
	char* field;
	char* p_delim = strstr(args, "---");
	int num_authors;
	int ref_num_authors;

	strcpy(params, args);

	*p_delim = '\0';
	field = p_delim + strlen("---");

	num_authors = get_number_of_authors_with_field(data, institution, field);

	fscanf(ref_file, "%d\n", &ref_num_authors);
	SD_TEST_INTS(num_authors, ref_num_authors, params);

	return 0;
}

/* Frees the histogram returned by get_histogram_of_citations! */
static int check_histogram(PublData* data, char* args, FILE* ref_file)
{
	int i;
	int num_years = 0;
	int* histogram = get_histogram_of_citations(data, atoll(args), &num_years);
	int ref_num_years;
	int ref_histogram_entry;

	fscanf(ref_file, "%d\n", &ref_num_years);
	SD_TEST_INTS(num_years, ref_num_years, args);

	for (i = 0; i != num_years; ++i)
	{
		fscanf(ref_file, "%d\n", &ref_histogram_entry);
		SD_TEST_INTS(histogram[i], ref_histogram_entry, args);
	}

	free(histogram);

	return 0;
}

/* Frees the memory allocated for the output of get_reading_order  */
static int check_reading_order(PublData* data, char* args, FILE* ref_file)
{
	int i;
	char ref_paper[MAX_BUFFER];
	int64_t id_paper = atoll(args);
	int distance = atoi(strchr(args, ' ') + 1);
	int num_papers = 0;
	int ref_num_papers;
	char** papers = get_reading_order(data, id_paper, distance,
		&num_papers);

	fscanf(ref_file, "%d\n", &ref_num_papers);
	SD_TEST_INTS(num_papers, ref_num_papers, args);

	for (i = 0; i < num_papers; ++i)
	{
		fgets(ref_paper, MAX_BUFFER, ref_file);
		ref_paper[strlen(ref_paper) - 1] = '\0';

		SD_TEST_STRINGS(papers[i], ref_paper, args);

		free(papers[i]);
	}
	free(papers);

	return 0;
}

static int check_find_best_coordinator(PublData* data, char* args,
	FILE* ref_file)
{
	char ref_best_coordinator[MAX_BUFFER];
	char* best_coordinator = find_best_coordinator(data, atoll(args));

	fgets(ref_best_coordinator, MAX_BUFFER, ref_file);
	ref_best_coordinator[strlen(ref_best_coordinator) - 1] = '\0';

	SD_TEST_STRINGS(best_coordinator, ref_best_coordinator, args);

	return 0;
}

int parse_commands(FILE* input_file, FILE* ref_file)
{
	char input[MAX_BUFFER];
	int ret = 0;
	char* args;
	char* callee;
	char *p_space;
	PublData* data;

	data = init_publ_data();
	DIE(data == NULL, "malloc data");

	while (!ret && fgets(input, MAX_BUFFER, input_file) != NULL)
	{
		callee = input;

		/* Convert the first space to '\0' for strcmp */
		p_space = strchr(input, ' ');
		*p_space = '\0';

		args = p_space + 1;
		/* Remove trailing '\n' */
		args[strlen(args) - 1] = '\0';

		if (!strcmp(callee, "add_paper"))
		{
			ret = run_add_paper(data, args);
		} else if (!strcmp(callee, "get_oldest_influence"))
		{
			ret = check_oldest_influence(data, args, ref_file);
		} else if (!strcmp(callee, "get_venue_impact_factor"))
		{
			ret = check_impact_factor(data, args, ref_file);
		} else if (!strcmp(callee, "get_number_of_influenced_papers"))
		{
			ret = check_influenced_papers(data, args, ref_file);
		} else if (!strcmp(callee, "get_erdos_distance"))
		{
			ret = check_erdos_distance(data, args, ref_file);
		} else if (!strcmp(callee, "get_most_cited_papers_by_field"))
		{
			ret = check_most_cited_papers(data, args, ref_file);
		} else if (!strcmp(callee, "get_number_of_papers_between_dates"))
		{
			ret = check_papers_between_dates(data, args, ref_file);
		} else if (!strcmp(callee, "get_number_of_authors_with_field"))
		{
			ret = check_authors_with_field(data, args, ref_file);
		} else if (!strcmp(callee, "get_histogram_of_citations"))
		{
			ret = check_histogram(data, args, ref_file);
		} else if (!strcmp(callee, "get_reading_order"))
		{
			ret = check_reading_order(data, args, ref_file);
		} else if (!strcmp(callee, "find_best_coordinator"))
		{
			ret = check_find_best_coordinator(data, args, ref_file);
		} else
		{
			DIE(1, "unknown function call");
		}
	}

	destroy_publ_data(data);

	return ret;
}
