// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef LINKED_HEAP_H
#define LINKED_HEAP_H

#include "stdlib.h"

typedef struct LINKED_HEAP_BLOCK_tag
{
  struct LINKED_HEAP_BLOCK_tag *next;
  unsigned size;
} LINKED_HEAP_BLOCK_t;

typedef struct
{
  long total_capacity;     // total size of the heap
  long alloc_requests;     // number of times alloc called
  long alloc_failures;     // how many times alloc has failed
  long free_requests;      // number of times free called with non-zero argument
  long free_failures;      // how many times free has been called with a bad argument or corrupt heap
  long used_bytes;         // how many bytes are used in the heap (a multiple of the allocation unit)
  long free_bytes;         // how many bytes are free in the heap
  long min_free_bytes;     // lowest number of free bytes bytes in the heap
  long fragmentation;      // number of non-contiguous free fragments (i.e. length of free list)
  long max_fragmentation;  // maximum length of free list ever recorded
  long chunk_size;         // Everything is allocated as a multiple of this size
} LINKED_HEAP_STATS_t;

typedef struct
{
  LINKED_HEAP_STATS_t stats;

  // Private fields, do not assume anything about these, nor change them...
  LINKED_HEAP_BLOCK_t *head;
  size_t size;
  size_t chunk_size;
} LINKED_HEAP_t;

int linked_heap_init(LINKED_HEAP_t *h, void *base, size_t size, size_t chunk_size);
void *linked_heap_alloc(LINKED_HEAP_t *h, size_t size);
void linked_heap_free(LINKED_HEAP_t *h, void *addr);

#endif
