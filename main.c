#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "cbuf.h"
#include "naldetector.h"

#define BUFSIZE 4096 //for reading stdin
#define HEADERS_MAXSIZE 32 // must be lower than BUFSIZE
#define PAYLOAD_START_PREFIX_NUMBER 3 // assume there are 2 NALs in headers

int main(int argc, char *argv[]) {
  struct cbuf cbuf;
  char buf[BUFSIZE];
  long cbufsize;
  int read;
  char *garbage;
  char *headers;
  int headers_size;
  struct naldetector detector;
  int remaining_start_prefixes;

  // parse commandline arguments
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

  // init circular buffer
  if (cbuf_init(&cbuf, cbufsize)) {
    fprintf(stderr, "Out of memory");
    exit(2);
  }

  // read headers
  // detect the i-th start prefix (0x000001), everything before is headers
  naldetector_init(&detector);
  headers_size = 0;
  remaining_start_prefixes = PAYLOAD_START_PREFIX_NUMBER;
  while (!feof(stdin) && !ferror(stdin) && remaining_start_prefixes > 0) {
    if ((read = fread(&buf[headers_size], 1, HEADERS_MAXSIZE - headers_size, stdin))) {
      while (read > 0 && remaining_start_prefixes > 0) {
        read--;
        headers_size++;
        if (naldetector_eat(&detector, buf[headers_size - 1])) {
          if (--remaining_start_prefixes == 0) {
            headers_size -= START_PREFIX_SIZE;
            if (!(headers = malloc(headers_size))) {
              perror("Cannot allocate headers buffer");
              exit(2);
            }
            memcpy(headers, buf, headers_size);
            // push the remaining data, including 0x000001, into the circular buffer
            cbuf_write(&cbuf, &buf[headers_size], read + START_PREFIX_SIZE);
          }
        }
      }
    }
  }

  if (remaining_start_prefixes == 0) {
    // the headers have been detected,

    // consume
    while (!feof(stdin) && !ferror(stdin)) {
      if ((read = fread(buf, 1, BUFSIZE, stdin))) {
        cbuf_write(&cbuf, buf, read);
      }
    }
    // write headers
    fwrite(headers, 1, headers_size, stdout);
    // write cbuf data
    cbuf_print_nals(&cbuf, stdout);
  }
  cbuf_free(&cbuf);
  return 0;
}
