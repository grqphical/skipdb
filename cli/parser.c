#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *skip_spaces(char *s) {
  while (*s == ' ' || *s == '\t')
    s++;
  return s;
}

static char *rstrip(char *s) {
  int len = strlen(s);
  while (len > 0 && isspace((unsigned char)s[len - 1])) {
    s[--len] = '\0';
  }
  return s;
}

static int is_positive_integer(const char *s) {
  if (*s == '\0')
    return 0;
  for (const char *p = s; *p; p++) {
    if (!isdigit((unsigned char)*p))
      return 0;
  }
  return strtol(s, NULL, 10) > 0;
}

void parse_and_run_command(skipdb *db, char *line) {
  int len = strlen(line);
  if (len < 3) {
    printf("error: unknown command\n");
    return;
  }

  rstrip(line); // trim trailing newline/whitespace from the raw line

  if (memcmp(line, "SET", 3) == 0) {
    char *rest = skip_spaces(line + 3);
    if (*rest == '\0') {
      printf("error: SET requires key=value\n");
      return;
    }

    char *eq = strchr(rest, '=');
    if (eq == NULL || eq == rest) {
      printf("error: SET requires key=value\n");
      return;
    }

    // trim trailing spaces off the key, then terminate it at '='
    char *key = rest;
    char *key_end = eq;
    while (key_end > key && isspace((unsigned char)*(key_end - 1)))
      key_end--;
    *key_end = '\0';
    if (*key == '\0') {
      printf("error: key cannot be empty\n");
      return;
    }

    char *value_start = skip_spaces(eq + 1);
    char *value;
    char *after_value; // where parsing continues after the value token

    if (*value_start == '"') {
      // quoted value: run until the next unescaped closing quote
      char *p = value_start + 1;
      char *closing = NULL;
      for (; *p != '\0'; p++) {
        if (*p == '"' && *(p - 1) != '\\') {
          closing = p;
          break;
        }
      }
      if (closing == NULL) {
        printf("error: unterminated quoted value\n");
        return;
      }

      value = value_start + 1; // points inside the quotes
      *closing = '\0';         // terminate value at closing quote
      after_value = closing + 1;

      if (*value == '\0') {
        printf("error: value cannot be empty\n");
        return;
      }
    } else {
      // unquoted value: no spaces allowed
      char *value_end = value_start;
      while (*value_end != '\0' && !isspace((unsigned char)*value_end))
        value_end++;

      value = value_start;
      after_value = (*value_end != '\0') ? value_end + 1 : value_end;
      *value_end = '\0';

      if (*value == '\0') {
        printf("error: value cannot be empty\n");
        return;
      }
    }

    long timestamp = 0; // sentinel: 0 means "not provided"
    char *ts_str = skip_spaces(after_value);
    if (*ts_str != '\0') {
      char *ts_end = ts_str;
      while (*ts_end != '\0' && !isspace((unsigned char)*ts_end))
        ts_end++;

      char *trailing = skip_spaces(ts_end);
      if (*trailing != '\0') {
        printf("error: too many arguments for SET\n");
        return;
      }

      *ts_end = '\0';
      if (!is_positive_integer(ts_str)) {
        printf("error: timestamp must be a positive integer\n");
        return;
      }
      timestamp = strtol(ts_str, NULL, 0);
    }

    skipdb_set(db, key, value, timestamp);

  } else if (memcmp(line, "GET", 3) == 0) {
    char *rest = skip_spaces(line + 3);
    if (*rest == '\0') {
      printf("error: GET requires key\n");
      return;
    }

    char *key = rest;
    char *key_end = key;
    while (*key_end != '\0' && !isspace((unsigned char)*key_end))
      key_end++;

    char *trailing = skip_spaces(key_end);
    if (*trailing != '\0') {
      printf("error: too many arguments for GET\n");
      return;
    }
    *key_end = '\0';

    char *value = skipdb_get(db, key);
    if (value == NULL) {
      printf("no value with given key found\n");
      return;
    } else {
      printf("\"%s\"\n", value);
      free(value);
    }

  } else if (memcmp(line, "DEL", 3) == 0) {
    char *rest = skip_spaces(line + 3);
    if (*rest == '\0') {
      printf("error: DEL requires key\n");
      return;
    }

    char *key = rest;
    char *key_end = key;
    while (*key_end != '\0' && !isspace((unsigned char)*key_end))
      key_end++;

    char *trailing = skip_spaces(key_end);
    if (*trailing != '\0') {
      printf("error: too many arguments for DEL\n");
      return;
    }
    *key_end = '\0';

    skipdb_delete(db, key);

  } else {
    printf("error: unknown command\n");
    return;
  }
}
