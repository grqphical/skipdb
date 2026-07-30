#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char *head;
  size_t size;
  size_t offset;
} ArenaAllocator;

extern uint8_t _arena_created;
extern ArenaAllocator _arena_allocator;

#define ARENA_ADDR(offset) ((void *)(_arena_allocator.head + offset))

// initializes the arena with a given size of memory
void arena_init(size_t size);
// frees all memory used by the arena
void arena_deinit(void);
// allocates memory in the arena, returning the numerical offset
size_t arena_allocate(size_t size);

#ifdef __cplusplus
}
#endif

#endif
