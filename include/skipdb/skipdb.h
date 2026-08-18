#ifndef SKIPDB_H
#define SKIPDB_H
#include "skipdb/export.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum skipdb_error_t { NONE, ALLOCATION_ERR };

typedef struct skipdb skipdb;

// Checks if an error occured within the library
SKIPDB_EXPORT bool skipdb_check_error(void);

// Prints an error message related to the last error reported by the library
SKIPDB_EXPORT const char *skipdb_get_error_message(void);

// creates a new SkipDB instance, where filepath is the file to persist the
// database to for an in-memory only database, set filepath to '<memory>'
SKIPDB_EXPORT void skipdb_open(const char *filepath, skipdb **db);
// frees all memory used by skipdb and writes all data to disk (if the database
// is set to persist)
SKIPDB_EXPORT void skipdb_close(skipdb *db);
// inserts a value at the given key in the database, if the key exists it simply
// overwrites it. If expiry is non-zero, they key will be removed from the
// database after the given number of seconds
SKIPDB_EXPORT void skipdb_set(skipdb *db, const char *key, const char *value,
                              uint32_t expiry);
// searches for a value in the database, if none is found the function returns
// NULL. NOTE: the memory returned by this function must be freed by the user,
// the user owns the memory.
SKIPDB_EXPORT char *skipdb_get(skipdb *db, const char *key);
// deletes a key in the database, if it doesn't exist the function is a no-op
SKIPDB_EXPORT void skipdb_delete(skipdb *db, const char *key);

#ifdef __cplusplus
}
#endif

#endif
