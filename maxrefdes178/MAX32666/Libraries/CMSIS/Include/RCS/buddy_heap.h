// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef BUDDY_HEAP_H
#define BUDDY_HEAP_H

#include <stdlib.h>

typedef struct buddy_link_tag
{
  struct buddy_link_tag *next;
  struct buddy_link_tag *prev;
} __BUDDY_LINKS_t;

typedef struct {
  long total_capacity;     // total size of the heap
  long alloc_requests;     // number of times alloc called
  long alloc_failures;     // how many times alloc has failed
  long free_requests;      // number of times free called with non-zero argument
  long free_failures;      // how many times free has been called with a bad argument or corrupt heap
  long used_bytes;         // how many bytes are used in the heap (a multiple of the allocation unit)
  long free_bytes;         // how many bytes are free in the heap
  long min_free_bytes;     // lowest number of free bytes bytes in the heap
  long blocks_split;       // how many times blocks have needed to be split.
  long blocks_coalesed;    // how many times blocks have been coalesced.
} BUDDY_HEAP_STATS_t;

typedef struct
{
  char *mem;
  unsigned max_k;     // 'm' in Fundamental Algorithms
  unsigned min_k;     // minimum block size returned
  unsigned min_size;  // 1 << min_k
  unsigned size;
  BUDDY_HEAP_STATS_t stats;
  __BUDDY_LINKS_t free_head[sizeof(void *)*8];  // 4 entries unused, could do something here...
} BUDDY_HEAP_t;


void *buddy_alloc(BUDDY_HEAP_t *h, size_t size);
void buddy_free(BUDDY_HEAP_t *h, void *data);
int buddy_heap_init(BUDDY_HEAP_t *h, void *mem, size_t size);

// For the future...
//
//typedef struct
//{
//  BUDDY_HEAP_t heap;
//  CTL_MUTEX_t mutex;
//  CTL_EVENT_SET_t events;
//} CTL_BUDDY_HEAP_t;
//
//int ctl_buddy_heap_init(CTL_BUDDY_HEAP_t *h, void *mem, size_t size);
//void *ctl_buddy_alloc(CTL_BUDDY_HEAP_t *h, size_t size, CTL_TIMEOUT_t timeout, CTL_);
//void *ctl_buddy_alloc_with_timeout(CTL_BUDDY_HEAP_t *h, size_t size, CTL_TIMEOUT_t timeout, CTL_TIME_t t);
//void ctl_buddy_free(CTL_BUDDY_HEAP_t *h, void *data);

#endif
