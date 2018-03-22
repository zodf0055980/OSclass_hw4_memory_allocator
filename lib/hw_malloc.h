#ifndef HW_MALLOC_H
#define HW_MALLOC_H

#include <sys/queue.h>
#include <stddef.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>

TAILQ_HEAD(tailhead,entry) bin[7];

struct chunk_header {
	TAILQ_ENTRY(chunk_header) entry;
	size_t pre_chunk_size;
	size_t chunk_size;
	int prev_free_flag;
};

extern void *hw_malloc(size_t bytes);
extern int hw_free(void *mem);
extern void *get_start_sbrk(void);

#endif
