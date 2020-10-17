CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -g
PUBL=publications
LINKEDLIST=data_structures/LinkedList
QUEUE=data_structures/Queue
LISTGRAPH=data_structures/ListGraph
HASHTABLE=data_structures/HashTable
TOOLS=data_structures/Tools
ALGORITHMS=data_structures/Algorithms

.PHONY: build clean

build: $(PUBL)_unlinked.o $(LINKEDLIST).o $(QUEUE).o $(HASHTABLE).o $(TOOLS).o $(ALGORITHMS).o
	ld -r $(PUBL)_unlinked.o $(LINKEDLIST).o $(QUEUE).o $(HASHTABLE).o $(TOOLS).o $(ALGORITHMS).o -g -o $(PUBL).o

$(PUBL)_unlinked.o: $(PUBL).c $(PUBL).h
	$(CC) $(CFLAGS) $(PUBL).c -c -o $(PUBL)_unlinked.o

$(LINKEDLIST).o: $(LINKEDLIST).c $(LINKEDLIST).h
	$(CC) $(CFLAGS) $(LINKEDLIST).c -c -o $(LINKEDLIST).o

$(QUEUE).o: $(QUEUE).c $(QUEUE).h
	$(CC) $(CFLAGS) $(QUEUE).c -c -o $(QUEUE).o

$(HASHTABLE).o: $(HASHTABLE).c $(HASHTABLE).h
	$(CC) $(CFLAGS) $(HASHTABLE).c -c -o $(HASHTABLE).o

$(TOOLS).o: $(TOOLS).c $(TOOLS).h
	$(CC) $(CFLAGS) $(TOOLS).c -c -o $(TOOLS).o

$(ALGORITHMS).o: $(ALGORITHMS).c $(ALGORITHMS).h
	$(CC) $(CFLAGS) $(ALGORITHMS).c -c -o $(ALGORITHMS).o

clean:
	rm -f *.o *.h.gch
	rm -f ./data_structures/*.o ./data_structures/*.h.gch
