#include <malloc.h>
#include <string.h>
#include "cbuf.h"
#include "naldetector.h"

int cbuf_init(struct cbuf *cbuf, size_t capacity) {
  cbuf->data = (char *) malloc(capacity);
  if (!cbuf->data) {
    return 1;
  }
  cbuf->capacity = capacity;
  cbuf->head = 0;
  cbuf->length = 0;
  return 0;
}

void cbuf_free(struct cbuf *cbuf) {
  free(cbuf->data);
}

size_t cbuf_write(struct cbuf *cbuf, char *data, size_t length) {
  size_t chunk;
  if (length > cbuf->capacity) {
    // the last bytes fill the whole cbuf
    memcpy(cbuf->data, &data[length - cbuf->capacity], cbuf->capacity);
    cbuf->head = 0;
    cbuf->length = cbuf->capacity;
    return length;
  }
  // update the cbuf length
  cbuf->length += length;
  if (cbuf->length > cbuf->capacity) {
    cbuf->length = cbuf->capacity;
  }
  if (cbuf->head + length > cbuf->capacity) {
    // copy the data in 2 chunks
    chunk = cbuf->capacity - cbuf->head;
    memcpy(&cbuf->data[cbuf->head], data, chunk);
    memcpy(cbuf->data, data, length - chunk);
    cbuf->head = length - chunk;
  } else {
    // copy the data in 1 chunk
    memcpy(&cbuf->data[cbuf->head], data, length);
    cbuf->head += length;
    if (cbuf->head == cbuf->capacity) {
      // if the chunk is right-aligned with the buffer
      cbuf->head = 0;
    }
  }
  return length;
}

size_t cbuf_print_nals(struct cbuf *cbuf, FILE * stream) {
  int tail = (cbuf->head + cbuf->capacity - cbuf->length) % cbuf->capacity;

  // seek to next 0x000001 pattern
  int length; // length of data to write, after garbage skipped
  int skipped = 0;
  char b; // current byte
  int detected = 0;
  struct naldetector detector;
  naldetector_init(&detector);
  while (skipped < cbuf->capacity && !detected) {
    b = cbuf->data[tail];
    tail++;
    if (tail == cbuf->capacity) {
      tail = 0;
    }
    skipped++;
    detected = naldetector_eat(&detector, b);
  }
  if (detected) {
    // rewind to write the 0x000001 pattern
    tail -= START_PREFIX_SIZE;
    skipped -= START_PREFIX_SIZE;
    if (tail < 0) {
      tail += cbuf->capacity;
    }
    // recompute the new length, after garbage has been skipped
    length = cbuf->length - skipped;
    if (tail + length > cbuf->capacity) {
      fwrite(&cbuf->data[tail], 1, cbuf->capacity - tail, stream);
      fwrite(cbuf->data, 1, length + tail - cbuf->capacity, stream);
    } else {
      fwrite(&cbuf->data[tail], 1, length, stream);
    }
  }
  return cbuf->length;
}
