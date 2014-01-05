#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cbuf.h"

#define BUFSIZE 4096 //for reading stdin

int main(int argc, char *argv[]) {
  struct cbuf cbuf;
  char buf[BUFSIZE];
  long cbufsize;
  int read;
  char *garbage = NULL;

  if (argc == 1) {
    cbufsize = 10 * 1024 * 1024; // 10mb
  } else if (argc == 2) {
    errno = 0;
    cbufsize = strtol(argv[1], &garbage, 0);
    if (errno) {
      perror("Cannot parse cbufsize argument");
      exit(1);
    }
    if (garbage[0] != '\0') {
      fprintf(stderr, "cbufsize contains garbage: %s\n", garbage);
      exit(1);
    }
    cbufsize *= 1024; // convert from kb
  } else {
    fprintf(stderr, "Syntax: %s [cbufsize in kb]\n", argv[0]);
    exit(1);
  }
  if (cbuf_init(&cbuf, cbufsize)) {
    fprintf(stderr, "Out of memory");
    exit(2);
  }
  while (!feof(stdin) && !ferror(stdin)) {
    if (read = fread(buf, 1, BUFSIZE, stdin)) {
      cbuf_write(&cbuf, buf, read);
    }
  }
  cbuf_print_nals(&cbuf, stdout);
  cbuf_free(&cbuf);
  return 0;
}
