#ifndef MEMALLOC_HEADER
#define MEMALLOC_HEADER

#include "globals.h"
#include "linked_list.h"
#include <unistd.h>
#include <sys/mman.h>

/* Allocates memory, address is returned. */
void *memalloc     (unsigned int size);

/* Calls memalloc and clears the memory block. */
void *clear_alloc  (unsigned int size);

/* Frees the memory block. */
int   memfree      (void *to_free);

/* Reallocates memory blocks given size, address is returned. */
void *rememalloc   (void *to_realloc, unsigned int size);


#define BRK_FAIL  (node_t*) -1
#define MMAP_FAIL (node_t*) -1

#define EBAD_SIZE (void*) -1
#define EBAD_ADDR -1

/* Manages heap allocation. */
node_t *heap_alloc (unsigned int size);

/* Manages memory mapped allocation. */
node_t *mmap_alloc (unsigned int size);

#define NULL_QWORD     0x0000000000000000
#define NULL_BYTE      0x00
#define QWAD_WORD_SIZE 8

/* Clears the requested memory block given it's address and size. */
void  mem_clear_64 (void *addr, unsigned int size);

/* Copy's the memory content from source to destination given addresses and
   sizes. */
void  mem_copy_64  (void *dst, void *src, unsigned int size);

#endif
