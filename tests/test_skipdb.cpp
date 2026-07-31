#include "skipdb.h"
#include <gtest/gtest.h>

TEST(SkipDBTests, TestInsertAndSearch) {
  skipdb_instance *db = skipdb_init("<memory>");

  skipdb_insert(db, "foo", "bar");
  char *result = skipdb_lookup(db, "foo");

  ASSERT_EQ(strcmp(result, "bar"), 0);

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

  skipdb_insert(db, "foo", "bar");

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
