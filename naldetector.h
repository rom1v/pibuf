#ifndef __H_NAL_DETECTOR
#define __H_NAL_DETECTOR

/* NAL detector is an automaton for detecting NAL start_prefixes (0x000001) in H.264 streams. */
struct naldetector {
  int pattern_bytes_ok;
};

void naldetector_init(struct naldetector *detector);

// return 1 on detection, 0 elsewhere.
int naldetector_eat(struct naldetector* detector, char c);
#endif
