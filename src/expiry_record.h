#ifndef EXPIRY_RECORD_H
#define EXPIRY_RECORD_H

#include <stddef.h>
#include <uthash.h>

typedef struct {
  const char *key;
  size_t expiration_timestamp;
  UT_hash_handle hh;
} ExpiryRecord;

#endif
