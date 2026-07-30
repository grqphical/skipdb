#include "arena_allocator.h"
#include <gtest/gtest.h>

TEST(ArenaAllocatorTests, TestInit) {
  arena_init(1024);

  ASSERT_EQ(_arena_created, 1);
  ASSERT_EQ(_arena_allocator.offset, 0);
  ASSERT_EQ(_arena_allocator.size, 1024);

  ASSERT_NE((size_t)_arena_allocator.head, 0);

  arena_deinit();
}

TEST(ArenaAllocatorTests, TestAllocate) {
  arena_init(1024);
  size_t offset = arena_allocate(64);
  ASSERT_EQ(offset, 0);
  size_t offset_2 = arena_allocate(128);
  ASSERT_EQ(offset_2, 64);
  arena_deinit();
}

TEST(ArenaAllocatorTests, TestAllocateBeyondSize) {
  arena_init(1024);
  size_t offset = arena_allocate(2056);
  ASSERT_EQ(offset, 0);
  ASSERT_EQ(_arena_allocator.size, 3072);
  arena_deinit();
}

TEST(ArenaAllocatorTests, TestOffsetToAddress) {
  arena_init(1024);
  size_t offset = arena_allocate(1);
  uint32_t *ptr = (uint32_t *)ARENA_ADDR(offset);
  ASSERT_EQ(ptr, (uint32_t *)(_arena_allocator.head + offset));
  *ptr = 57;
  ASSERT_EQ(*(uint32_t *)(_arena_allocator.head + offset), 57);
  arena_deinit();
}

TEST(ArenaAllocatorTests, TestDeInit) {
  arena_init(1024);
  arena_deinit();
  ASSERT_EQ(_arena_created, 0);
  ASSERT_EQ((size_t)_arena_allocator.head, 0);
}
