#ifndef __H_CIRCULAR_BUFFER
#define __H_CIRCULAR_BUFFER

#include <stddef.h>

struct cbuf {
  char *data;
  size_t capacity;
  int head;
  size_t length;
};

int cbuf_init(struct cbuf *cbuf, size_t length);
void cbuf_free(struct cbuf *cbuf);
size_t cbuf_write(struct cbuf *cbuf, char *data, size_t length);
size_t cbuf_print_nals(struct cbuf *cbuf, FILE *stream);

#endif
