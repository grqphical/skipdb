#include "parser.h"
#include "skipdb/skipdb.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLI_LINE_BUF_SIZE 1024
char *read_line(void) {
  int bufsize = CLI_LINE_BUF_SIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "skipdb_cli: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    if (position >= bufsize) {
      bufsize += CLI_LINE_BUF_SIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "skipdb_cli: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

int main(int argc, char **argv) {
  skipdb *db;
  if (argc == 1) {
    skipdb_open("<memory>", &db);
    if (skipdb_check_error()) {
      skipdb_print_error();
    }

  } else if (argc == 2) {
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
      printf("usage: skipdb_cli <filename>\n");
      return 0;
    }

    printf("this feature has not been implemented yet\n");
    return 0;
  } else {
    printf("usage: skipdb_cli <filename>\n");
    return -1;
  }

  char *line;

  printf("skipdb_cli - type ':exit' to quit\n");
  do {
    printf("skipdb> ");
    line = read_line();

    if (strcmp(line, ":exit") == 0) {
      free(line);
      break;
    }

    parse_and_run_command(db, line);
    if (skipdb_check_error()) {
      skipdb_print_error();
    }

    free(line);

  } while (true);

  skipdb_close(db);
  free(db);

  return 0;
}
