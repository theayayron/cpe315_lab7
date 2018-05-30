
#include <stdint.h>

#ifndef MATMUL2_H
#define MATMUL2_H

typedef struct _cache_line {
  uint8_t v;
  uint64_t tag; 
} cache_line;


#endif
