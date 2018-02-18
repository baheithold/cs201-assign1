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
static void swapNodeValues(BSTNODE *x, BSTNODE *y);
static void heapify(HEAP *h, BSTNODE *root);
static void bubbleDown(HEAP *h, BSTNODE *n);


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
    void (*swapNodeValues)(BSTNODE *, BSTNODE *);
    void (*heapify)(HEAP *, BSTNODE *);
    void (*bubbleDown)(HEAP *, BSTNODE *);
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
    h->insertionQueue = newQUEUE(h->display, h->free);
    h->extractionStack = newSTACK(h->display, h->free);
    h->display = d;
    h->compare = c;
    h->free = f;
    h->swapNodeValues = swapNodeValues;
    h->heapify = heapify;
    h->bubbleDown = bubbleDown;
    return h;
}


/*
 *  Method: insertHEAP
 *  Usage:  insertHEAP(h, newINTEGER(7));
 *  Description: This method inserts a generic value into an un-heapified heap.
 *  No fixing up of the heap should occur. This method runs in constant time. 
 */
void insertHEAP(HEAP *h, void *value) {
    // TODO: Am I setting bst-size correctly?
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
void buildHEAP(HEAP *h) {
    assert(h != 0);
    h->heapify(h, getBSTroot(h->tree));
    // Move remaining nodes on insertionQueue to extractionStack
    while (sizeQUEUE(h->insertionQueue) > 0) {
        push(h->extractionStack, dequeue(h->insertionQueue));
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
    if (h->size == 1) {
        h->size--;
        void *rv = getBSTNODEvalue(getBSTroot(h->tree));
        pruneLeafBST(h->tree, getBSTroot(h->tree));
        return rv;
    }
    BSTNODE *popped = pop(h->extractionStack);
    h->swapNodeValues(getBSTroot(h->tree), popped);
    pruneLeafBST(h->tree, popped);
    h->heapify(h, getBSTroot(h->tree));
    h->size--;
    return getBSTNODEvalue(popped);
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


void swapNodeValues(BSTNODE *x, BSTNODE *y) {
    // TODO: Am I correct?
    assert(x != 0 && y != 0);
    void *tmp = getBSTNODEvalue(x);
    setBSTNODEvalue(x, getBSTNODEvalue(y));
    setBSTNODEvalue(y, tmp);
}


void heapify(HEAP *h, BSTNODE *root) {
    if (root == NULL) return;
    h->heapify(h, getBSTNODEleft(root));    // heapify left subtree of root
    h->heapify(h, getBSTNODEright(root));   // heapify right subtree of root
    h->bubbleDown(h, root);
}


void bubbleDown(HEAP *h, BSTNODE *n) {
    assert(n != 0);
    BSTNODE *smallest = n;
    BSTNODE *lChild = getBSTNODEleft(n);
    BSTNODE *rChild = getBSTNODEright(n);
    if (lChild != NULL) {
        if (h->compare(getBSTNODEvalue(lChild), getBSTNODEvalue(n)) < 0) {
            smallest = lChild;
        }
    }
    if (rChild != NULL) {
        if (h->compare(getBSTNODEvalue(rChild), getBSTNODEvalue(smallest)) < 0) {
            smallest = rChild;
        }
    }
    if (h->compare(getBSTNODEvalue(n), getBSTNODEvalue(smallest)) != 0) {
        h->swapNodeValues(n, smallest);
        h->bubbleDown(h, smallest);
    }
}
