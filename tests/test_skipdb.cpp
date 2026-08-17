#include "skipdb/skipdb.h"
#include <gtest/gtest.h>

TEST(SkipDBTests, TestInsertAndSearch) {
  skipdb *db;
  skipdb_open("<memory>", &db);

  skipdb_set(db, "foo", "bar", 0);
  char *result = skipdb_get(db, "foo");

  ASSERT_EQ(strcmp(result, "bar"), 0);

  skipdb_close(db);
  free(db);
  free(result);
}

TEST(SkipDBTests, TestInsertExpiry) {
  skipdb *db;
  skipdb_open("<memory>", &db);

  skipdb_set(db, "foo", "bar", 3);
  skipdb_set(db, "bar", "foo", 10);

  printf("inserted value, sleeping for five seconds to test expiry...\n");
  sleep(5);

  char *result = skipdb_get(db, "foo");

  ASSERT_EQ((size_t)result, NULL);
  free(result);

  result = skipdb_get(db, "bar");

  ASSERT_EQ(strcmp(result, "foo"), 0);
  free(result);

  skipdb_close(db);
  free(db);
}

TEST(SkipDBTests, TestSearchOfNonExistentValue) {
  skipdb *db;
  skipdb_open("<memory>", &db);

  char *result = skipdb_get(db, "foo");

  ASSERT_EQ((size_t)result, NULL);

  skipdb_close(db);
  free(db);
  free(result);
}

TEST(SkipDBTests, TestDeletion) {
  skipdb *db;
  skipdb_open("<memory>", &db);

  skipdb_set(db, "foo", "bar", 0);

  skipdb_delete(db, "foo");

  char *result = skipdb_get(db, "foo");

  ASSERT_EQ((size_t)result, NULL);

  skipdb_close(db);
  free(db);
  free(result);
}

TEST(SkipDBTests, TestDeletionOfNonExistentValue) {
  skipdb *db;
  skipdb_open("<memory>", &db);

  skipdb_delete(db, "foo");

  skipdb_close(db);
  free(db);
}
