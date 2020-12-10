Network Academy - Copyright Micu Florian-Luis, Fatu Miruna-Maria, Buf Sorina Anamaria 2020

Given a set of papers as JSON files, add them to a database and query them.
Each task has a different filter for a query.

To run the checker, simply run the command "./checker.sh".
To compile the program use the command "make -f Makefile_tema3" and to run
a custom test run the executable "./tema3" with two arguments. The first
argument is the input file and the second one is the ref file.

Note: The checker verifies if all tests are passed under 1, 3 and 5 seconds
depending on the difficulty of the tests. To check a file runtime use the
command "time ./tema3 x y" where "x" and "y" are the input and ref file.
Our implementation runs under 4 seconds at worst depending on the OS and
CPU. I recommend using native Linux (not installed on a VM) to see the best
results.

A paper has the following attributes:
1. title
2. ID
3. publication year
4. name of the conference that the paper was originally published
5. study domains
6. list of authors
  - name of author
  - id of author
  - institutions of author
7. references
8. citations (added by me to help with some tasks)

Task 0: Add all papers
Added all the papers in a HashTable that works as a database (key -> the 
paper's ID, value -> paper).

Task 1: Get oldest influence
In order to return the oldest influence, we used the BFS algorithm on the
references.

Task 2: Get venue impact factor
This tasks requires the program to return an impact factor of a paper
by calculating the average number of citations of all the papers 
published in a certain journal at the query time. To do this, we used
a HashTable for venues (key -> name of the venue, value -> paper id)
and we calculated the average impact factor.

Task 3: Get number of influenced papers
Just like task 1, in order the return the number of influenced papers
for a given depth, we used the BFS algorithm on the citations.

Task 4: Get Erdos distance
The Erdos distance is definied as the distance between two authors.
we started by creating an authors HashTable (key -> name of author, 
value -> list of papers where he is one of the authors) and from this we
applied the BFS algorithm for all the co-authors he had for each paper
to get the distance.

Task 5: Get most citated paper by field
We used a HashTable for fields (key -> field name, value -> list of papers
with said field) and we applied the MergeSort algorithm to sort the list
by the number of citations.

Task 6: Get number of papers between dates
A frequency vector is used to store the number of papers published in a
certain year, after which we make the sum between the number of papers
published between two years.

Task 7: Get number of authors with field
Since we made the field HashTable, we used it again to get the authors
that published a paper in a certain field. While we count the authors, 
we make sure their institution matches the one given in the parameter.

Task 8: Get histogram of citations
For this task, we used the authors hashtable to get an author's
publication. Moreover, we create the histogram array by simply
creating a frequency vector that uses as index the distance between
the current year (2020) and the paper's year.

Task 9: Get Reading Order
TO DO - this task is more complicated than we taught, it uses the BFS
algorithm as its base, however it has some intricacies that we found
in this olympiad problem: https://www.infoarena.ro/problema/cezar

Task 10: Find best coordinator
TO DO - using the other tasks, return the best coordinator using a
specific formula.



