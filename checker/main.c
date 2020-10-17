#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "runner.h"

int main(int argc, char* argv[])
{
	int ret;
	FILE *input_file, *ref_file;

	if (argc != 3)
	{
		printf("Usage:\n\t%s input_file ref_file\n", argv[0]);
		return -1;
	}

	input_file = fopen(argv[1], "rt");
	DIE(input_file == NULL, "input file");

	ref_file = fopen(argv[2], "rt");
	DIE(ref_file == NULL, "ref file");

	ret = parse_commands(input_file, ref_file);

	fclose(input_file);
	fclose(ref_file);

	return ret;
}
