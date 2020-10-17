#ifndef RUNNER_H_
#define RUNNER_H_

#include <stdio.h>

#include "publications.h"
#include "parson.h"

/**
 * Reads each command (query or new publication) from an input file and makes the
 * required call to the API.
 * After the call returns its result, the function compares this result with the
 * one found in the ref file.
 *
 * @param input_file	the file where commands will be read from
 * @param ref_file		the file containing the results that the tested
 *						functions will be compared against
 * @return				a code symbolising the correctness of the execution:
 *						* 0: success
 *						* -1: failure
 */
int parse_commands(FILE* input_file, FILE* ref_file);

#endif  /* RUNNER_H_ */
