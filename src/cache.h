#ifndef CACHE_GLOB
#define CACHE_GLOB

#include <stdlib.h>
#include <stddef.h>

#define MAX_CHUNKS 256
#define CHUNK_SIZE 4
#define INIT_CACHE 4
#define CACHE_GROW 4

typedef struct {
	void** pointers;
	int index;
	int current_size;
	int chunk_count;
} Allocator;

extern Allocator glob_alloc[MAX_CHUNKS];

void* request(Allocator* a);
void giveback(Allocator* a, void* );
void initalize_allocator();

#define REQUEST(type) ((type*) request(&glob_alloc[sizeof(type)/CHUNK_SIZE]));
#define PUTBACK(type, ptr) giveback(&glob_alloc[sizeof(type)/CHUNK_SIZE], ptr);

#endif