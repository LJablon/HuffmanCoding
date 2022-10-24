#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "pqueue.h"
#include "pack.h"

typedef struct pqueue {
    int count; 
    int length; 
    void **data; 
    int (*compare)();
}PQ;

/*
    Function: Creates Queue by intializing all variables and returning a pointer to the queue
    time complexity O(1)

    Question: do we need a written type for data or is a single generic variable? 
    In other words, is it like COEN11 where we are 

*/
PQ *createQueue(int (*compare)()){

    PQ *pq = malloc(sizeof(PQ));
    assert(pq != NULL); 

    pq->count = 0;
    pq->length = 10; 
    pq->data = malloc(sizeof(void*) * pq->length);
    assert(pq->data != NULL); 
    pq->compare = compare; 

    return pq; 
}

/* 
    Function: frees the memory of the pq->data array and the pq pointer
    Time complexity: O(1)
*/

void destroyQueue(PQ *pq) {
    assert(pq != NULL); 
    free(pq->data);
    free(pq);
}

/*
    Function: returns the number of elements in the queue 
    Time complexity: O(1)
*/ 

int numEntries(PQ *pq) {
    assert(pq != NULL); 
    return pq->count; 
}

/*
    time complexity: O(logn)
*/ 

void addEntry(PQ *pq, void *entry) {

    assert(pq != NULL && entry != NULL); 

    if (pq->count > pq->length) {
        pq->length *= 2;  
        pq->data = realloc(pq->data, sizeof(void*) * pq->length); 
        assert(pq->data != NULL); 
    }

    pq->data[pq->count] = entry; 
     

    int child = pq->count;
    void *temp;  
    int parent = (child-1)/2;

 
    while (pq->count > 0 && parent >= 0 && (*pq->compare)(pq->data[parent], pq->data[child]) > 0) { /* better exit condition? */
        
        temp = pq->data[child];
        pq->data[child] = pq->data[parent];
        pq->data[parent] = temp; 
       
        child = parent;
        parent = ((child-1)/2);
    }    
    pq->count++;


}
/*


    time complexity 0(logn)
*/
void *removeEntry(PQ *pq) {
    
    assert(pq != NULL); 
    
    if (pq->count == 0) {
        return NULL; 
    }
  
    //pop operation for minheap
    void *item = pq->data[0];
    pq->data[0] = pq->data[pq->count-1];
    pq->count--; 

  
    int parent = 0; 
    int L_child, R_child, smaller; 
    void *temp;  
    

    //while there is aleft child to be compared
    while((2*parent)+1 < ((pq->count))) {
    
        L_child = (2*parent)+1; 
        R_child = (2*parent)+2; 

        //if there is both a right and a left child 
        if ((2*parent)+2 < (pq->count-1) ) {
           
            //find smaller child (left or right) if left smaller then -->
            if ((*pq->compare)(pq->data[L_child], pq->data[R_child]) < 0) {
                smaller = (2*parent)+1;
            }
            
            //if right is smaller make smaller index 
            else {
                smaller = ((2*parent)+2); 
            }
        }

        //if there is only a left child, then only compare the left 
        else {
            smaller = (2*parent)+1;
        }

        //if the parent is not smaller then swap
        if ((parent >= 0) && (*pq->compare)(pq->data[parent], pq->data[smaller]) > 0)  {
                temp = pq->data[smaller];
                pq->data[smaller] = pq->data[parent];
                pq->data[parent] = temp; 
        }

        //if parent is smallest then break
        else {
            break; 
        }
        parent = smaller;
    }
   
    return item; 
}



/*
static void swap (void *a, void *b) {
    void *temp = a; 
    a = b; 
    b = temp; 

}
*/