#include <stdio.h>
#include <stdlib.h>
#include "cbuf.h"

#define BUFSIZE 4096 //for reading stdin

int main(int argc, char *argv[]) {
  struct cbuf cbuf;
  char buf[BUFSIZE];
  int read;
  if (cbuf_init(&cbuf, 10000000)) {
    fprintf(stderr, "Out of memory");
    exit(1);
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
