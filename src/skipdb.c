#include "skipdb.h"
#include "arena_allocator.h"
#include <stdlib.h>
#include <string.h>

#define __in_memory_name "<memory>"
#define __default_value_store_size 1024

skipdb_instance *skipdb_init(const char *filepath) {
  if (memcmp(filepath, __in_memory_name, 8) != 0) {
    // the user is not requesting an in memory database, so open the file for
    // appending
    return NULL;
  }
  skipdb_instance *db = malloc(sizeof(skipdb_instance));
  db->sl = sl_init();
  db->value_allocator = arena_init(__default_value_store_size);

  return db;
}

void skipdb_destroy(skipdb_instance *db) {
  sl_free(db->sl);
  arena_free(db->value_allocator);
  free(db);
}

void skipdb_insert(skipdb_instance *db, const char *key, const char *value) {
  size_t key_length = strlen(key);
  size_t value_length = strlen(value);

  char *value_clone = (char *)arena_allocate(db->value_allocator, value_length);
  strcpy(value_clone, value);

  sl_insert(db->sl, key, key_length, value_clone, value_length);
}

char *skipdb_lookup(skipdb_instance *db, const char *key) {
  size_t key_len = strlen(key);
  SkipListNode *node = sl_search(db->sl, key, key_len);
  if (node == NULL)
    return NULL;

  char *value_clone = malloc(sizeof(node->value_len));
  strcpy(value_clone, node->value_ptr);

  return value_clone;
}

void skipdb_delete(skipdb_instance *db, const char *key) {
  size_t key_len = strlen(key);
  sl_delete(db->sl, key, key_len);
}
