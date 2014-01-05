#include <stdio.h>
#include "naldetector.h"

#define NAL_UNIT_TYPE_MASK 0x1f
#define NAL_UNIT_TYPE_IDR 5

void naldetector_init(struct naldetector *detector, int detect_idr_only) {
  detector->pattern_bytes_ok = 0;
  detector->detect_idr_only = detect_idr_only;
}

int naldetector_eat(struct naldetector *detector, char c) {
  if (detector->pattern_bytes_ok == START_PREFIX_SIZE - 1) {
    if (c == 0x01) {
      // found, give the response if we don't need to know the idr
      if (detector->detect_idr_only) {
        // will be detecting on the next byte, depending of the NAL unit type
        detector->pattern_bytes_ok = START_PREFIX_SIZE;
        return 0;
      }
      // detected, START_PREFIX_SIZE to rewind
      return START_PREFIX_SIZE;
    }
    if (c != 0x00) {
      detector->pattern_bytes_ok = 0;
    }
  } else if (detector->pattern_bytes_ok == START_PREFIX_SIZE) {
    // special case after the prefix has been detecting for reading the NAL unit type
    // fortunately, this NAL unit type is in the very next byte
    if (!detector->detect_idr_only) {
      fprintf(stderr, "Warning: this naldetector must be reinitialized before detecting again");
    }
    if ((c & NAL_UNIT_TYPE_MASK) == NAL_UNIT_TYPE_IDR) {
      return START_PREFIX_SIZE + 1;
    }
    // reset for seeking the next NAL
    detector->pattern_bytes_ok = 0;
    return 0;
  } else if (c == 0x00) {
    detector->pattern_bytes_ok++;
  } else {
    detector->pattern_bytes_ok = 0;
  }
  return 0;
}
