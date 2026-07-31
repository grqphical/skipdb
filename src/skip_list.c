#include "skip_list.h"
#include "arena_allocator.h"
#include <string.h>

/* Total order over (key, key_length) pairs: lexicographic on bytes,
 * with a shorter-but-equal-prefix key sorting before the longer one. */
static int sl_compare(const char *key_a, size_t len_a, const char *key_b,
                      size_t len_b) {
  size_t min_len = len_a < len_b ? len_a : len_b;
  int cmp = min_len ? memcmp(key_a, key_b, min_len) : 0;
  if (cmp != 0)
    return cmp;
  if (len_a < len_b)
    return -1;
  if (len_a > len_b)
    return 1;
  return 0;
}

static int sl_random_level(void) {
  int level = 1;
  while (level < MAX_LEVELS && (rand() & 1)) {
    level++;
  }
  return level;
}

SkipList *sl_init(void) {

  SkipList *sl = (SkipList *)malloc(sizeof(SkipList));
  ArenaAllocator *arena = arena_init(DEFAULT_ALLOCATION_SIZE);
  sl->head = (SkipListNode *)arena_allocate(arena, sizeof(SkipListNode));

  sl->head->key = NULL;
  sl->head->key_length = 0;

  sl->allocator = arena;
  return sl;
}

SkipListNode *sl_search(SkipList *sl, const char *key, size_t key_length) {
  SkipListNode *node = sl->head;
  for (int level = MAX_LEVELS - 1; level >= 0; level--) {
    while (node->next[level] != NULL &&
           sl_compare(node->next[level]->key, node->next[level]->key_length,
                      key, key_length) < 0) {
      node = node->next[level];
    }
  }

  node = node->next[0];
  if (node != NULL && !node->dead &&
      sl_compare(node->key, node->key_length, key, key_length) == 0) {
    return node;
  }
  return NULL;
}

SkipListNode *sl_insert(SkipList *sl, const char *key, size_t key_length) {
  SkipListNode *update[MAX_LEVELS];
  SkipListNode *node = sl->head;

  for (int level = MAX_LEVELS - 1; level >= 0; level--) {
    while (node->next[level] != NULL &&
           sl_compare(node->next[level]->key, node->next[level]->key_length,
                      key, key_length) < 0) {
      node = node->next[level];
    }
    update[level] = node;
  }

  node = node->next[0];

  if (node != NULL &&
      sl_compare(node->key, node->key_length, key, key_length) == 0) {
    node->key = key;
    node->key_length = key_length;
    node->dead = false;
    return node;
  }

  SkipListNode *new_node =
      (SkipListNode *)arena_allocate(sl->allocator, sizeof(SkipListNode));
  if (new_node == NULL)
    return NULL;

  new_node->key = key;
  new_node->key_length = key_length;
  new_node->dead = false;
  for (int i = 0; i < MAX_LEVELS; i++) {
    new_node->next[i] = NULL;
  }

  int new_level = sl_random_level();
  for (int level = 0; level < new_level; level++) {
    new_node->next[level] = update[level]->next[level];
    update[level]->next[level] = new_node;
  }

  return new_node;
}

void sl_delete(SkipList *sl, const char *key, size_t key_length) {
  SkipListNode *node = sl->head;

  for (int level = MAX_LEVELS - 1; level >= 0; level--) {
    while (node->next[level] != NULL &&
           sl_compare(node->next[level]->key, node->next[level]->key_length,
                      key, key_length) < 0) {
      node = node->next[level];
    }
  }

  node = node->next[0];
  if (node != NULL && !node->dead &&
      sl_compare(node->key, node->key_length, key, key_length) == 0) {
    node->dead = true;
  }
}

void sl_free(SkipList *sl) {
  if (sl == NULL)
    return;

  arena_free(sl->allocator);
  free(sl);
}
