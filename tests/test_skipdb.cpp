#include "skipdb/skipdb.h"
#include <gtest/gtest.h>

TEST(SkipDBTests, TestInsertAndSearch) {
  skipdb_instance *db = skipdb_init("<memory>");

  skipdb_insert(db, "foo", "bar", 0);
  char *result = skipdb_lookup(db, "foo");

  ASSERT_EQ(strcmp(result, "bar"), 0);

  skipdb_destroy(db);
}

TEST(SkipDBTests, TestInsertExpiry) {
  skipdb_instance *db = skipdb_init("<memory>");

  skipdb_insert(db, "foo", "bar", 3);
  skipdb_insert(db, "bar", "foo", 10);

  printf("inserted value, sleeping for five seconds to test expiry...\n");
  sleep(5);

  char *result = skipdb_lookup(db, "foo");

  ASSERT_EQ((size_t)result, NULL);

  result = skipdb_lookup(db, "bar");

  ASSERT_EQ(strcmp(result, "foo"), 0);

  skipdb_destroy(db);
}

TEST(SkipDBTests, TestSearchOfNonExistentValue) {
  skipdb_instance *db = skipdb_init("<memory>");

  char *result = skipdb_lookup(db, "foo");

  ASSERT_EQ((size_t)result, NULL);

  skipdb_destroy(db);
}

TEST(SkipDBTests, TestDeletion) {
  skipdb_instance *db = skipdb_init("<memory>");

  skipdb_insert(db, "foo", "bar", 0);

  skipdb_delete(db, "foo");

  char *result = skipdb_lookup(db, "foo");

  ASSERT_EQ((size_t)result, NULL);

  skipdb_destroy(db);
}

TEST(SkipDBTests, TestDeletionOfNonExistentValue) {
  skipdb_instance *db = skipdb_init("<memory>");

  skipdb_delete(db, "foo");

  skipdb_destroy(db);
}
