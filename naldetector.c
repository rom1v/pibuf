#include "naldetector.h"

void naldetector_init(struct naldetector *detector) {
  detector->pattern_bytes_ok = 0;
}

int naldetector_eat(struct naldetector *detector, char c) {
  if (detector->pattern_bytes_ok == 2) {
    if (c == 0x01) {
      // found, the detector will need to be init for reuse
      return 1;
    }
    if (c != 0x00) {
      detector->pattern_bytes_ok = 0;
    }
  } else if (c == 0x00) {
    detector->pattern_bytes_ok++;
  } else {
    detector->pattern_bytes_ok = 0;
  }
  return 0;
}
