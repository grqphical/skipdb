#include "skip_list.h"
#include <gtest/gtest.h>

TEST(SkipListTests, TestSkipListCreation) {
  SkipList *sl = sl_init();
  ASSERT_NE((size_t)sl, NULL);
  sl_free(sl);
}

TEST(SkipListTests, TestSkipListInsertionAndSearch) {
  SkipList *sl = sl_init();
  ASSERT_NE((size_t)sl, NULL);

  sl_insert(sl, "foo", 3, NULL, 0);
  sl_insert(sl, "bar", 3, NULL, 0);

  SkipListNode *foo = sl_search(sl, "foo", 3);
  ASSERT_NE((size_t)foo, 0);
  ASSERT_EQ(foo->key_length, 3);
  ASSERT_EQ(memcmp(foo->key, "foo", 3), 0);

  SkipListNode *bar = sl_search(sl, "bar", 3);
  ASSERT_NE((size_t)bar, NULL);
  ASSERT_EQ(bar->key_length, 3);
  ASSERT_EQ(memcmp(bar->key, "bar", 3), 0);

  SkipListNode *baz = sl_search(sl, "baz", 3);
  ASSERT_EQ((size_t)baz, NULL);

  sl_free(sl);
}

TEST(SkipListTests, TestSkipListDeletion) {
  SkipList *sl = sl_init();
  ASSERT_NE((size_t)sl, NULL);

  sl_insert(sl, "foo", 3, NULL, 0);
  sl_insert(sl, "bar", 3, NULL, 0);

  sl_delete(sl, "bar", 3);

  SkipListNode *bar = sl_search(sl, "bar", 3);
  ASSERT_EQ((size_t)bar, NULL);

  SkipListNode *foo = sl_search(sl, "foo", 3);
  ASSERT_NE((size_t)foo, 0);
  ASSERT_EQ(foo->key_length, 3);
  ASSERT_EQ(memcmp(foo->key, "foo", 3), 0);

  sl_free(sl);
}
