#! /bin/bash

MAKEFILE=Makefile_tema3
ELF=tema3
TIMEOUTS=("" 1 3 5 1 3 5 1 3 5 1 3 5 1 3 5 1 3 5 1 3 5 1 3 5 1 3 5 1 3 5)
TOTAL_POINTS=0
INPUTS=tests_in
REFS=tests_ref
POINTS=("" 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3)
POINTS_README=10
POINTS_CODING_STYLE=10
DOTS="................................................................................"

make -f $MAKEFILE clean
make -f $MAKEFILE build

if [ $? -ne "0" ]; then
	printf "\nThere were build errors...";
	printf "\nHomework score: ${TOTAL_POINTS}\n\n";

	exit 1;
fi

printf "\nLoading checker...\n"
find parsed_papers/ -type f | xargs cat > /dev/null
printf "Done loading checker!\n\n"

printf "\n======================== Checking Homework Assignment 3 ========================\n\n"

# Check tests
i=0

for f in $INPUTS/*; do
	first_part="${i}) ${f:9}";
	last_part="PASSED";

	i=$((i + 1));

	# { } supresses the "Segmentation fault" message
	{ timeout ${TIMEOUTS[$i]} ./$ELF $f $REFS/${f:9}; }

	exit_code=$?;

	if [ $exit_code -ne "0" ]; then
		if [ $exit_code -eq "124" ]; then
			last_part="TIME LIMIT EXCEEDED [0/${POINTS[$i]}]";
		elif [ $exit_code -eq "255" ]; then
			last_part="WRONG ANSWER [0/${POINTS[$i]}]";
		else
			last_part="RUNTIME ERROR [0/${POINTS[$i]}]";
		fi

		printf "%s%s%s\n" "$first_part" "${DOTS:$((${#last_part} + ${#first_part}))}" "$last_part"
		continue;
	fi

	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 -q ./$ELF $f $REFS/${f:9} &> /dev/null;

	if [ $? -ne "0" ]; then
		last_part="VALGRIND ERROR [0/${POINTS[$i]}]";
	else
		last_part="PASSED [${POINTS[$i]}/${POINTS[$i]}]";
		TOTAL_POINTS=$((TOTAL_POINTS + POINTS[$i]));
	fi

	printf "%s%s%s\n" "$first_part" "${DOTS:$((${#last_part} + ${#first_part}))}" "$last_part";
done

# Check README
first_part="README"

if [ $TOTAL_POINTS -gt 0 ]; then
	find . -maxdepth 1 -type f -name "README*" | egrep '.*' &> /dev/null

	if [ $? -eq "0" ]; then
		last_part="EXISTS [${POINTS_README}/${POINTS_README}]"
		TOTAL_POINTS=$((TOTAL_POINTS + $POINTS_README))
	else
		last_part="DOES NOT EXIST [0/${POINTS_README}]"
	fi
else
	last_part="NO TESTS PASSED [0/${POINTS_README}]"
fi

printf "%s%s%s\n" "$first_part" "${DOTS:$((${#last_part} + ${#first_part}))}" "$last_part"

# Check coding styile
first_part="CODING STYLE"

if [ $TOTAL_POINTS -gt 0 ]; then
	# Exclude checker files
	find . -regextype egrep -regex ".*(\.h|\.c)" \
	| egrep -v "(main\.c|parson\..|runner\..)" \
	| xargs ./cpplint.py --filter=-readability/casting,-build/header_guard,-build/include &> checkstyle.txt;

	if [ $? -eq "0" ]; then
		last_part="OK [${POINTS_CODING_STYLE}/${POINTS_CODING_STYLE}]"
		TOTAL_POINTS=$((TOTAL_POINTS + $POINTS_CODING_STYLE))
	else
		last_part="NOT OK - see checkstyle.txt [0/${POINTS_CODING_STYLE}]"
	fi
else
	last_part="NO TESTS PASSED [0/${POINTS_CODING_STYLE}]"
fi

printf "%s%s%s\n" "$first_part" "${DOTS:$((${#last_part} + ${#first_part}))}" "$last_part"

printf "\n==================== Finished Checking Homework Assignment 3 ===================\n"

printf "\nHomework score: ${TOTAL_POINTS}/110\n\n"

make -f $MAKEFILE clean
