OBJS = integer.o heap.o bst.o stack.o queue.o dll.o sll.o test-heap.o
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g

all: heapsort

heapsort:	$(OBJS)
		gcc $(LOPTS) $(OBJS) -o heapsort

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

heap.o:	heap.c heap.h bst.h queue.h stack.h sll.h dll.h
		gcc $(OOPTS) heap.c

heapsort.o:	heapsort.c heap.h
		gcc $(OOPTS) heapsort.c

test:	heapsort
		@./heapsort

valgrind: heapsort
		valgrind ./heapsort

clean:
		rm -f vgcore.* $(OBJS) heapsort
