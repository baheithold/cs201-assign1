/*
 *  File:   heap.c
 *  Author: Brett Heithold
 *  Description:
 */


#include "heap.h"
#include "queue.h"
#include "stack.h"
#include "bst.h"
#include <stdlib.h>
#include <assert.h>


// Private HEAP method prototypes
static int isRoot(HEAP *h, BSTNODE *n);
static void swapNodeValues(BSTNODE *x, BSTNODE *y);
static void heapify(HEAP *h, BSTNODE *n);


struct HEAP {
    BST *tree;
    int size;

    QUEUE *insertionQueue;
    STACK *extractionStack;

    // Public Methods
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*free)(void *);
    // Private Methods
    int (*isRoot)(HEAP *h, BSTNODE *);
    void (*swapNodeValues)(BSTNODE *, BSTNODE *);
    void (*heapify)(HEAP *, BSTNODE *);
};


/*
 *  Constructor:    newHEAP
 *  Usage:  HEAP *h = newHEAP(displayINTEGER, compareINTEGER, freeINTEGER);
 *  Description:
 */
HEAP *newHEAP(
    void (*d)(void *, FILE *),
    int (*c)(void *, void *),
    void (*f)(void *)) {
    HEAP *h = malloc(sizeof(HEAP));
    assert(h != 0);
    h->tree = newBST(d, c, NULL, f);
    h->size = 0;
    h->insertionQueue = newQUEUE(d, NULL);
    h->extractionStack = newSTACK(d, NULL);
    h->display = d;
    h->compare = c;
    h->free = f;
    h->isRoot = isRoot;
    h->swapNodeValues = swapNodeValues;
    h->heapify = heapify;
    return h;
}


/*
 *  Method: insertHEAP
 *  Usage:  insertHEAP(h, newINTEGER(7));
 *  Description: This method inserts a generic value into an un-heapified heap.
 *  No fixing up of the heap should occur. This method runs in constant time.
 */
void insertHEAP(HEAP *h, void *value) {
    assert(h != 0);
    BSTNODE *temp = newBSTNODE(value);
    if (sizeBST(h->tree) == 0) {
        // If tree is empty, add new node to root
        setBSTroot(h->tree, temp);
    }
    else {
        BSTNODE *front = peekQUEUE(h->insertionQueue);
        if (getBSTNODEleft(front) == NULL) {
            // If node at front of queue does not have left child
            // set the left child to be the new node and set the parent
            // of the new node
            setBSTNODEleft(front, temp);
            setBSTNODEparent(temp, front);
        }
        else if (getBSTNODEright(front) == NULL) {
            // If node at front of queue does not have right child
            // set the right child to be the new node and set the parent
            // of the new node
            setBSTNODEright(front, temp);
            setBSTNODEparent(temp, front);
        }
        if (getBSTNODEleft(front) != NULL && getBSTNODEright(front) != NULL) {
            // The front node already has two children, dequeue the front node
            push(h->extractionStack, dequeue(h->insertionQueue));
        }
    }
    enqueue(h->insertionQueue, temp);
    h->size++;
    setBSTsize(h->tree, h->size);
}


/*
 *  Method: buildHEAP
 *  Usage:  buildHEAP(h);
 *  Description:This method imparts heap ordering on the current heap.
 *  Once buildHEAP has been called, no insertions to the heap should be made.
 *  This method runs in linear time.
 */
/*
void buildHEAP(HEAP *h) {
    assert(h != 0);
    BSTNODE *dequeued;
    BSTNODE *parent;
    while (sizeQUEUE(h->insertionQueue) > 0) {
        dequeued = dequeue(h->insertionQueue);
        parent = getBSTNODEparent(dequeued);
        while (!h->isRoot(parent)) {
            h->heapify(h, parent);
            parent = getBSTNODEparent(parent);
        }
        push(h->extractionStack, dequeued);
    }
}
*/
void buildHEAP(HEAP *h) {
    assert(h != 0);
    BSTNODE *dequeued;
    BSTNODE *parent;
    while (sizeQUEUE(h->insertionQueue) > 0) {
        dequeued = dequeue(h->insertionQueue);
        parent = getBSTNODEparent(dequeued);
        while (parent != NULL) {
            h->heapify(h, parent);
            if (h->isRoot(h, parent)) break;
            parent = getBSTNODEparent(parent);
        }
        push(h->extractionStack, dequeued);
    }
}


/*
 *  Method: peekHEAP
 *  Usage:  void *val = peekHEAP(h);
 *  Description:This method returns the value at the “root” of the heap;
 *  the heap is unmodified. This method runs in constant time.
 */
void *peekHEAP(HEAP *h) {
    assert(h != 0);
    return getBSTNODEvalue(getBSTroot(h->tree));
}


/*
 *  Method: extractHEAP
 *  Usage:  void *val = extractHEAP(h);
 *  Description: This method returns the value at the “root” of the heap,
 *  rebuilding the heap. This method runs in logarithmic time.
 */
void *extractHEAP(HEAP *h) {
    assert(h != 0);
    void *rv = getBSTNODEvalue(getBSTroot(h->tree));
    BSTNODE *popped = pop(h->extractionStack);
    if (h->size > 1) {
        setBSTNODEvalue(getBSTroot(h->tree), getBSTNODEvalue(popped));
        h->heapify(h, getBSTroot(h->tree));
    }
    pruneLeafBST(h->tree, popped);
    h->size--;
    setBSTsize(h->tree, h->size);
    return rv;
}


/*
 *  Method: sizeHEAP
 *  Usage:  int size = sizeHEAP(h);
 *  Description: This method returns the number of values currently in the tree.
 *  It runs in constant time.
 */
int sizeHEAP(HEAP *h) {
    assert(h != 0);
    return h->size;
}


/*
 *  Method: displayHEAP
 *  Usage:  displayHEAP(h, stdout);
 *  Description: This method calls the display method of the underlying
 *  data structure (BST).
 *  Example Output: [7 [9] [10]]
 */
void displayHEAP(HEAP *h, FILE *fp) {
    assert(h != 0);
    displayBST(h->tree, fp);
}


/*
 *  Method: displayHEAPdebug
 *  Usage:  displayHEAPdebug(h, stdout);
 *  Description: This method should print the size of the heap, the size of the
 *  underlying data structure, and call the debug display method of the
 *  underlying data structure.
 *  Example Output:
 */
void displayHEAPdebug(HEAP *h, FILE *fp) {
    assert(h != 0);
    fprintf(fp, "heap size: %d\n", h->size);
    fprintf(fp, "bst size: %d\n", sizeBST(h->tree));
    displayBSTdebug(h->tree, fp);
    printf("size stack: %d\n", sizeSTACK(h->extractionStack));
}


/*
 *  Method: freeHEAP
 *  Usage:  freeHEAP(h);
 *  Description: This method frees the heap, in part by freeing the underlying
 *  data structure.
 */
void freeHEAP(HEAP *h) {
    freeBST(h->tree);
    freeQUEUE(h->insertionQueue);
    freeSTACK(h->extractionStack);
    free(h);
}


/****************************** Private Methods ******************************/


int isRoot(HEAP *h, BSTNODE *n) {
    return (n == getBSTroot(h->tree)) ? 1 : 0;
    //return (getBSTNODEparent(n) == NULL) ? 1 : 0;
}


void swapNodeValues(BSTNODE *x, BSTNODE *y) {
    assert(x != 0 && y != 0);
    void *tmp = getBSTNODEvalue(x);
    setBSTNODEvalue(x, getBSTNODEvalue(y));
    setBSTNODEvalue(y, tmp);
}


void heapify(HEAP *h, BSTNODE *n) {
    BSTNODE *left = getBSTNODEleft(n);
    BSTNODE *right = getBSTNODEright(n);
    BSTNODE *smallest = n;
    if (left != NULL) {
        if (h->compare(getBSTNODEvalue(left), getBSTNODEvalue(n)) < 0) {
            smallest = left;
        }
    }
    if (right != NULL) {
        if (h->compare(getBSTNODEvalue(right), getBSTNODEvalue(smallest)) < 0) {
            smallest = right;
        }
    }
    if (h->compare(getBSTNODEvalue(smallest), getBSTNODEvalue(n)) != 0) {
        h->swapNodeValues(n, smallest);
        h->heapify(h, smallest);
    }
}
