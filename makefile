OBJS = integer.o real.o string.o heap.o bst.o stack.o queue.o dll.o sll.o \
	   scanner.o heapsort.o
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g

all: heapsort

heapsort:	$(OBJS)
		gcc $(LOPTS) $(OBJS) -o heapsort

integer.o:	integer.c integer.h
		gcc $(OOPTS) integer.c

real.o:	real.c real.h
		gcc $(OOPTS) real.c

string.o:	string.c string.h
		gcc $(OOPTS) string.c

queue.o:	queue.c queue.h sll.h
		gcc $(OOPTS) queue.c

stack.o:	stack.c stack.h dll.h
		gcc $(OOPTS) stack.c

sll.o:	sll.c sll.h
		gcc $(OOPTS) sll.c

dll.o:	dll.c dll.h
		gcc $(OOPTS) dll.c

bst.o:	bst.c bst.h queue.h
		gcc $(OOPTS) bst.c

heap.o:	heap.c heap.h bst.h queue.h stack.h sll.h dll.h
		gcc $(OOPTS) heap.c

scanner.o:	scanner.c scanner.h
		gcc $(OOPTS) scanner.c

heapsort.o:	heapsort.c heap.h
		gcc $(OOPTS) heapsort.c

test-all:	heapsort
		@echo Testing heapsort -i -D hs-1-0.data...
		@./heapsort -i -D ./Testing/hs-1-0.data > ./Testing/myresults/hs-1-0.txt
		@diff ./Testing/expectedresults/hs-1-0.txt ./Testing/myresults/hs-1-0.txt
		@echo Testing heapsort -r -D hs-1-1.data...
		@./heapsort -r -D ./Testing/hs-1-1.data > ./Testing/myresults/hs-1-1.txt
		@diff ./Testing/expectedresults/hs-1-1.txt ./Testing/myresults/hs-1-1.txt
		@echo Testing heapsort -s -D hs-1-2.data...
		@./heapsort -s -D ./Testing/hs-1-2.data > ./Testing/myresults/hs-1-2.txt
		@diff ./Testing/expectedresults/hs-1-2.txt ./Testing/myresults/hs-1-2.txt
		@echo Testing heapsort hs-1-3.data...
		@./heapsort ./Testing/hs-1-3.data > ./Testing/myresults/hs-1-3.txt
		@diff ./Testing/expectedresults/hs-1-3.txt ./Testing/myresults/hs-1-3.txt
		@echo Testing heapsort hs-1-4.data...
		@./heapsort ./Testing/hs-1-4.data > ./Testing/myresults/hs-1-4.txt
		@diff ./Testing/expectedresults/hs-1-4.txt ./Testing/myresults/hs-1-4.txt
		@echo Testing heapsort hs-1-5.data...
		@./heapsort ./Testing/hs-1-5.data > ./Testing/myresults/hs-1-5.txt
		@diff ./Testing/expectedresults/hs-1-5.txt ./Testing/myresults/hs-1-5.txt
		@echo Testing heapsort hs-1-6.data...
		@./heapsort ./Testing/hs-1-6.data > ./Testing/myresults/hs-1-6.txt
		@diff ./Testing/expectedresults/hs-1-6.txt ./Testing/myresults/hs-1-6.txt
		@echo Testing heapsort hs-1-7.data...
		@./heapsort ./Testing/hs-1-7.data > ./Testing/myresults/hs-1-7.txt
		@diff ./Testing/expectedresults/hs-1-7.txt ./Testing/myresults/hs-1-7.txt
		@echo Testing -v option...
		@./heapsort -v

test:	heapsort
		@echo Testing heapsort -i -D hs-1-0.data...
		@./heapsort -i -D ./Testing/hs-1-0.data > ./Testing/myresults/hs-1-0.txt
		@diff ./Testing/expectedresults/hs-1-0.txt ./Testing/myresults/hs-1-0.txt
		@echo Testing heapsort -r -D hs-1-1.data...
		@./heapsort -r -D ./Testing/hs-1-1.data > ./Testing/myresults/hs-1-1.txt
		@diff ./Testing/expectedresults/hs-1-1.txt ./Testing/myresults/hs-1-1.txt
		@echo Testing heapsort -s -D hs-1-2.data...
		@./heapsort -s -D ./Testing/hs-1-2.data > ./Testing/myresults/hs-1-2.txt
		@diff ./Testing/expectedresults/hs-1-2.txt ./Testing/myresults/hs-1-2.txt
		@echo Testing heapsort hs-1-3.data...
		@./heapsort ./Testing/hs-1-3.data > ./Testing/myresults/hs-1-3.txt
		@diff ./Testing/expectedresults/hs-1-3.txt ./Testing/myresults/hs-1-3.txt

valgrind: heapsort
		valgrind ./heapsort ./Testing/hs-0-0.data

clean:
		rm -f vgcore.* $(OBJS) heapsort
