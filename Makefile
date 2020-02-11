test: Node.o LIST.o Memory.o test.o
	gcc -o test Node.o LIST.o Memory.o test.o
test.o: test.c LIST.h Memory.h
	gcc -c test.c
Memory.o: Memory.c Memory.h
	gcc -c Memory.c
LIST.o: LIST.c LIST.h Node.h Memory.h
	gcc -c LIST.c
Node.o: Node.c Node.h Memory.h
	gcc -c Node.c
clean:
	rm *.o test