#include "arena_allocator.h"

uint8_t _arena_created = 0;
ArenaAllocator _arena_allocator;

size_t max(size_t a, size_t b) {
  if (a > b)
    return a;
  else
    return b;
}

void arena_init(size_t size) {
  _arena_allocator.size = size;
  _arena_allocator.offset = 0;

  void *head = malloc(size);
  if (head == NULL) {
    perror("malloc()");
    exit(1);
  }
  _arena_allocator.head = head;
  _arena_created = 1;
}

void arena_deinit(void) {
  if (!_arena_created) {
    return;
  }

  free(_arena_allocator.head);
  _arena_created = 0;
  _arena_allocator.head = NULL;
}

size_t arena_allocate(size_t size) {
  if (!_arena_created) {
    printf("error: arena not initialized");
    exit(1);
  }

  // reallocate space if we don't have enough
  size_t remaining = _arena_allocator.size - _arena_allocator.offset;
  if (size > remaining) {
    size_t deficit =
        size - remaining; // extra bytes needed beyond current capacity

    size_t extra_blocks =
        (deficit + _arena_allocator.size - 1) / _arena_allocator.size; // ceil
    size_t new_size = max(_arena_allocator.size * (1 + extra_blocks),
                          _arena_allocator.size * 2);

    char *new_head = realloc(_arena_allocator.head, new_size);
    if (new_head == NULL) {
      perror("realloc()");
      exit(1);
    }
    _arena_allocator.head = new_head;
    _arena_allocator.size = new_size;
  }

  size_t offest_to_return = _arena_allocator.offset;
  _arena_allocator.offset += size;

  return offest_to_return;
}
