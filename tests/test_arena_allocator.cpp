#include "arena_allocator.h"
#include <gtest/gtest.h>

TEST(ArenaAllocatorTests, TestInit) {
  arena_init(1024);

  ASSERT_EQ(_arena_created, 1);
  ASSERT_EQ(_arena_allocator.offset, 0);
  ASSERT_EQ(_arena_allocator.size, 1024);

  ASSERT_NE((size_t)_arena_allocator.head, 0);
}

TEST(ArenaAllocatorTests, TestAllocate) {
  size_t offset = arena_allocate(64);
  ASSERT_EQ(offset, 0);
  size_t offset_2 = arena_allocate(128);
  ASSERT_EQ(offset_2, 64);
}

TEST(ArenaAllocatorTests, TestAllocateBeyondSize) {
  size_t offset = arena_allocate(2048);
  ASSERT_EQ(offset, 64 + 128);
  ASSERT_EQ(_arena_allocator.size, 3072);
}

TEST(ArenaAllocatorTests, TestDeInit) {
  arena_deinit();
  ASSERT_EQ(_arena_created, 0);
  ASSERT_EQ((size_t)_arena_allocator.head, 0);
}
