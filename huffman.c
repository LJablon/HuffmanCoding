#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>  
#include "pqueue.h"
#include "pack.h"
#define SIZE 257


typedef struct node NODE;

//combines children nodes 
static NODE *mkNode(int data, NODE *l, NODE *r) {
    NODE *np; 
    np = malloc(sizeof(NODE));
    
    if (l != NULL) {
        l->parent = np; 
    }
   
    if (r != NULL) {
        r->parent = np; 
    }
    np->count = data; 
    return np; 
}

//compare function
static int intcmp (int *i1, int *i2) 
{
    return ((*i1 < *i2) ? -1: (*i1 > *i2)); 
}

int main (int argc, char *argv[]) 
{
    if (argc != 3 ) {
        printf("Please include two files\n");
        return 0; 
    }

    //open first file to be read
    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("File could not be open. Please try again\n");
        return 0; 
    }

    //integer array to track of charcter frequency
    int counts[SIZE];
    int i; 
    for(i=0; i<SIZE; i++) {
        counts[i] = 0; 
    }
    
    //count character frequency from file1
    int ch; 
    while(!feof(fp)) {
        ch = fgetc(fp); 
        counts[ch]++; 
    }

    PQ *pq = createQueue(intcmp); 

    NODE *leafnodes[SIZE]; 

    
    for (i = 0; i<SIZE; i++) {
        leafnodes[i] = NULL; 
    }

    //if an index of counts is not NULL
    for(i = 0; i<SIZE; i++) {
        
        if (counts[i] != 0) {
            NODE *np = malloc(sizeof(NODE));
            assert(np != NULL); 
            leafnodes[i] = np; 
            np->count = counts[i];
        }
    }

    //add the nonzero pointers to the priority queue
    for(i=0; i<SIZE; i++) {
        if (leafnodes[i] != NULL ){
            NODE *p = malloc(sizeof(NODE));
            p->count = counts[i];
            addEntry(pq, p);
            free(p);  
        }
    }

    //combine nodes to build the tree
    while(numEntries(pq) > 0)
    {
        NODE *ln = malloc(sizeof(NODE));
        NODE *rn = malloc(sizeof(NODE)); 
        NODE *parent = malloc(sizeof(NODE));
        ln = removeEntry(pq); 
        rn = removeEntry(pq);
        parent = mknode((ln->count + rn->count), ln, rn);
        free(ln); 
        free(rn); 
        free(parent);
    }
    
    //pack goes here 

    destroyQueue(pq);
  
    return 0; 
}