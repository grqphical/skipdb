#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "arena_allocator.h"

#define MAX_LEVELS 8

typedef struct sl_node {
  const char *key;
  size_t key_length;
  struct sl_node *next[MAX_LEVELS];
  bool dead;
} SkipListNode;

typedef struct {
  SkipListNode *head;
} SkipList;

SkipList *sl_init(void);
SkipListNode *sl_search(SkipList *sl, const char *key, size_t key_length);
SkipListNode *sl_insert(SkipList *sl, const char *key, size_t key_length);
void sl_delete(SkipList *sl, const char *key, size_t key_length);

#ifdef __cplusplus
}
#endif

#endif
