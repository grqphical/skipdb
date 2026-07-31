#include "arena_allocator.h"
#include <gtest/gtest.h>

TEST(ArenaAllocatorTests, TestInit) {
  ArenaAllocator *arena = arena_init(1024);

  ASSERT_EQ(arena->offset, 0);
  ASSERT_NE((size_t)arena->head, NULL);
  ASSERT_EQ(arena->size, 1024);

  arena_free(arena);
}

TEST(ArenaAllocatorTests, TestAllocate) {
  ArenaAllocator *arena = arena_init(1024);

  uint32_t *ptr = (uint32_t *)arena_allocate(arena, sizeof(uint32_t));
  *ptr = 576;

  ASSERT_NE((size_t)ptr, NULL);
  ASSERT_EQ(*ptr, 576);
  ASSERT_EQ(ARENA_OFFSET(arena, (char *)ptr), 0);
}

TEST(ArenaAllocatorTests, TestAllocateBeyondSize) {
  ArenaAllocator *arena = arena_init(1024);
  char *big_chunk = arena_allocate(arena, 2048);

  ASSERT_NE((size_t)big_chunk, 0);
  ASSERT_EQ(arena->size, 1024);
  ASSERT_NE((size_t)arena->next, NULL);
  ASSERT_EQ(arena->next->size, 2048);

  arena_free(arena);
}
