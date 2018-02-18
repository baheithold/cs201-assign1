OBJS = integer.o heap.o bst.o stack.o queue.o dll.o sll.o test-heap.o
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g

all: test-heap

test-heap:	$(OBJS)
		gcc $(LOPTS) $(OBJS) -o test-heap

integer.o:	integer.c integer.h
		gcc $(OOPTS) integer.c

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

heap.o:	heap.c heap.h bst.h queue.h stack.h
		gcc $(OOPTS) heap.c

test-heap.o:	test-heap.c heap.h
		gcc $(OOPTS) test-heap.c

test:	test-heap
		@./test-heap

valgrind: test-heap
		valgrind ./test-heap

clean:
		rm -f vgcore.* $(OBJS) test-heap
