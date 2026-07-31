#include "arena_allocator.h"

#define __default_new_block_size 1024

size_t max(size_t a, size_t b) {
  if (a > b)
    return a;
  else
    return b;
}

ArenaAllocator *arena_init(size_t size) {
  ArenaAllocator *arena = malloc(sizeof(ArenaAllocator));
  if (arena == NULL) {
    perror("arena: malloc()");
    exit(1);
  }
  arena->size = size;
  arena->offset = 0;
  arena->next = NULL;

  char *ptr = (char *)calloc(1, size);
  arena->head = ptr;
  return arena;
}

void arena_free(ArenaAllocator *arena) {
  ArenaAllocator *current_arena = arena;
  while (current_arena != NULL) {
    ArenaAllocator *next_arena = current_arena->next;
    free(current_arena->head);
    free(current_arena);
    current_arena = next_arena;
  }
}

char *arena_allocate(ArenaAllocator *arena, size_t size) {
  ArenaAllocator *tail = arena;
  while (tail->next != NULL) {
    tail = tail->next;
  }

  size_t remaining = tail->size - tail->offset;
  if (size > remaining) {
    size_t new_block_size = max(size, __default_new_block_size);
    ArenaAllocator *new_arena = arena_init(new_block_size);
    tail->next = new_arena;
    tail = new_arena;
  }

  char *ptr_to_return = (char *)(tail->head + tail->offset);
  tail->offset += size;
  return ptr_to_return;
}
