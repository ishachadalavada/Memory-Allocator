#define _DEFAULT_SOURCE
#define _BSD_SOURCE 
#include <malloc.h> 
#include <stdio.h> 
#include <unistd.h>

// Include any other headers we need here

// NOTE: You should NOT include <stdlib.h> in your final implementation

#include <debug.h> // definition of debug_printf
typedef struct block {
  size_t size;        // How many bytes beyond this block have been allocated in the heap
  struct block *next; // Where is the next block in your linked list
  int free;           // Is this memory free, i.e., available to give away?, free = 1, not free = 0 
} block_t;

#define BLOCK_SIZE sizeof(block_t)

//initializes head of linked list
block_t* block_head = NULL;


// allocates memory block of size s
void *mymalloc(size_t s) {
/*
  void *p = (void *) malloc(s); // In your solution no calls to malloc should be
                               // made! Determine how you will request memory :)
*/

  // variable to iterate through the linked list
  block_t* temp_next = block_head;

  // iterates through to find a block in the already created memory
  while(temp_next != NULL && temp_next->next != NULL) {
    // if found returns the memory address and sets to not free
    if(temp_next->free == 1 && temp_next->size >= s) {
      temp_next->free = 0;
      void *p = (void*) temp_next + BLOCK_SIZE;
      return p;
    }
    temp_next = temp_next->next;
  }

  // if no blocks available, creates one
  block_t *b = (block_t *) sbrk(BLOCK_SIZE + s);

  // if sbrk runs out of memory or an error happens, returns NULL
  if(b==NULL) {
    return NULL;
  }

  // sets new memory block's header information
  b->size = s;
  // add to the front of the list
  b->next = block_head;
  b->free = 0;

  // changes the head to link it to the front
  block_head = b;

  debug_printf("malloc %zu bytes\n", s);

  // converts to void pointer type
  void* p = (void*) b + BLOCK_SIZE;
  
  return p;
}


// allocated many memory blocks(nmemb) or size s
void *mycalloc(size_t nmemb, size_t s) {

 // void *p = (void *) calloc(nmemb, s); // In your solution no calls to calloc should be
                                       // made! Determine how you will request memory :)
 // creates an array of void *
  void * arr[nmemb]; 
  for (int i = 0; i < nmemb; i++) {
    arr[i] = mymalloc(s);
    if (arr[i] == NULL) {
      return NULL;
    }
  }

  debug_printf("calloc %zu bytes\n", s);

  return arr;
}


// frees blocks that were previously allocated
void myfree(void *ptr) {
  
  block_t* temp = (block_t*) ptr - 1;
  temp->free = 0;
  debug_printf("Freed some memory\n");

  // Replace the free below with your own free implementation
  // free(ptr);
}
