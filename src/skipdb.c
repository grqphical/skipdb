#include "skipdb/skipdb.h"
#include "arena_allocator.h"
#include "expiry_record.h"
#include "skipdb_internal.h"
#include "uthash.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define __in_memory_name "<memory>"
#define __default_value_store_size 1024

enum skipdb_error_t skipdb_err = NONE;

bool skipdb_check_error(void) { return skipdb_err != NONE; }

void skipdb_print_error(void) {
  switch (skipdb_err) {
  case NONE:
    break;
  case ALLOCATION_ERR:
    fprintf(stderr, "memory allocation failure\n");
    break;
  }
  skipdb_err = NONE;
}

void skipdb_open(const char *filepath, skipdb **db) {
  if (filepath != NULL && strcmp(filepath, __in_memory_name) != 0) {
    // the user is not requesting an in memory database, so open the file for
    // appending
    return;
  }

  *db = malloc(sizeof(skipdb));
  if (*db == NULL) {
    skipdb_err = ALLOCATION_ERR;
    return;
  }

  (*db)->sl = sl_init();
  (*db)->value_allocator = arena_init(__default_value_store_size);
  (*db)->expiry_table = NULL;

  return;
}

void skipdb_close(skipdb *db) {
  if (db == NULL)
    return;
  sl_free(db->sl);
  arena_free(db->value_allocator);

  ExpiryRecord *current_record;
  ExpiryRecord *tmp;

  HASH_ITER(hh, db->expiry_table, current_record, tmp) {
    HASH_DEL(db->expiry_table, current_record);
    free(current_record->key);
    free(current_record);
  }
}

void skipdb_set(skipdb *db, const char *key, const char *value,
                uint32_t expiry) {
  size_t key_length = strlen(key);
  size_t value_length = strlen(value);

  char *value_clone =
      (char *)arena_allocate(db->value_allocator, value_length + 1);
  strcpy(value_clone, value);

  sl_insert(db->sl, key, key_length, value_clone, value_length + 1);

  ExpiryRecord *existing;
  HASH_FIND(hh, db->expiry_table, key, key_length, existing);
  if (existing) {
    HASH_DEL(db->expiry_table, existing);
    free(existing->key);
    free(existing);
  }

  if (expiry > 0) {
    ExpiryRecord *record = malloc(sizeof(ExpiryRecord));
    if (record == NULL) {
      skipdb_err = ALLOCATION_ERR;
      return;
    }
    record->key = malloc(key_length + 1);
    strcpy(record->key, key);

    size_t current_timestamp = time(NULL);
    record->expiration_timestamp = current_timestamp + expiry;

    HASH_ADD_KEYPTR(hh, db->expiry_table, key, key_length, record);
  }
}

char *skipdb_get(skipdb *db, const char *key) {
  size_t key_len = strlen(key);
  ExpiryRecord *record;
  HASH_FIND_STR(db->expiry_table, key, record);

  if (record) {
    size_t current_timestamp = time(NULL);
    if (current_timestamp >= record->expiration_timestamp) {
      HASH_DEL(db->expiry_table, record);
      free(record->key);
      free(record);
      return NULL;
    }
  }

  SkipListNode *node = sl_search(db->sl, key, key_len);
  if (node == NULL) {
    return NULL;
  }
  char *value_clone = malloc(node->value_len + 1);
  strcpy(value_clone, node->value_ptr);

  return value_clone;
}

void skipdb_delete(skipdb *db, const char *key) {
  size_t key_len = strlen(key);
  sl_delete(db->sl, key, key_len);

  ExpiryRecord *record;
  HASH_FIND_STR(db->expiry_table, key, record);

  if (record) {
    HASH_DEL(db->expiry_table, record);
    free(record->key);
    free(record);
  }
}
