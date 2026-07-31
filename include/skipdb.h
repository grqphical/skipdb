#ifndef SKIPDB_H
#define SKIPDB_H
#include "arena_allocator.h"
#include "skip_list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  SkipList *sl;
  ArenaAllocator *value_allocator;
} skipdb_instance;

// creates a new SkipDB instance, where filepath is the file to persist the
// database to for an in-memory only database, set filepath to '<memory>'
skipdb_instance *skipdb_init(const char *filepath);
// frees all memory used by skipdb and writes all data to disk (if the database
// is set to persist)
void skipdb_destroy(skipdb_instance *db);
// inserts a value at the given key in the database, if the key exists it simply
// overwrites it
void skipdb_insert(skipdb_instance *db, const char *key, const char *value);
// searches for a value in the database, if none is found the function returns
// NULL. NOTE: the memory returned by this function must be freed by the user,
// the user owns the memory.
char *skipdb_lookup(skipdb_instance *db, const char *key);
// deletes a key in the database, if it doesn't exist the function is a no-op
void skipdb_delete(skipdb_instance *db, const char *key);

#ifdef __cplusplus
}
#endif

#endif
