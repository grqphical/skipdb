#include "arena_allocator.h"
#include "expiry_record.h"
#include "skip_list.h"
#include "skipdb/export.h"

struct skipdb {

  SkipList *sl;
  ExpiryRecord *expiry_table;
  ArenaAllocator *value_allocator;
};
