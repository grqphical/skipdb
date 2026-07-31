#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arena_allocator {
  char *head;
  size_t size;
  size_t offset;
  struct arena_allocator *next;
} ArenaAllocator;

#define ARENA_OFFSET(arena, addr) ((size_t)(addr - arena->head))

// initializes an arena with the size the arena should allocate
ArenaAllocator *arena_init(size_t size);
// frees all memory used by the arena
void arena_free(ArenaAllocator *arena);
// allocates memory in the arena, returning the pointer where the memory will be
// stored
char *arena_allocate(ArenaAllocator *, size_t size);

#ifdef __cplusplus
}
#endif

#endif
