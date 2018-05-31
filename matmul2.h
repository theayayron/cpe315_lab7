
#include <stdint.h>

#ifndef MATMUL2_H
#define MATMUL2_H

typedef struct _cache_line {
  uint8_t v;
  uint64_t tag; 
} cache_line;

int isolate_bits(long base, int start, int end);
void mem_write(int *mp);
void mem_read(int *mp);
void extract_fields(int *mp, uint8_t *index, uint8_t *offset, uint64_t *tag);
void init_cache(void);
void matmul( int r1, int c1, int c2 );

#endif
