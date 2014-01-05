#ifndef __H_NAL_DETECTOR
#define __H_NAL_DETECTOR

/* NAL detector is an automaton for detecting NAL start_prefixes (0x000001) in H.264 streams. */

/* IDR is a NAL unit type value (=5) indicating that no frame after it can reference any frame before it.
 * Therefore, this is a very good NAL candidate for cutting the video ;-) */

#define START_PREFIX_SIZE 4

struct naldetector {
  int pattern_bytes_ok;
  int detect_idr_only;
};

void naldetector_init(struct naldetector *detector, int detect_idr_only);

// return the number of bytes to rewind on detection, 0 elsewhere.
int naldetector_eat(struct naldetector* detector, char c);

#endif
