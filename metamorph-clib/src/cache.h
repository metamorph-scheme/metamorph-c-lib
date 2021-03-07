#ifndef CACHE_GLOB
#define CACHE_GLOB

#define MAX_CHUNKS 256
#define CHUNK_SIZE 4
#define INIT_CACHE 4
#define CACHE_GROW 4
#define CACHE_OFF

typedef struct {
	void** pointers;
	int index;
	int current_size;
	int chunk_count;
} allocator_t;

extern allocator_t glob_alloc[MAX_CHUNKS];

void* request(allocator_t* a);
void giveback(allocator_t* a, void* );
void initalize_allocator();

#define REQUEST(TYPE) ((TYPE*) request(&glob_alloc[sizeof(TYPE)/CHUNK_SIZE]))
#define REQUEST_ARRAY(TYPE, NUMBER) ((TYPE*) request(&glob_alloc[(sizeof(TYPE)*NUMBER)/CHUNK_SIZE]))
#define RELEASE(TYPE, PTR) giveback(&glob_alloc[sizeof(TYPE)/CHUNK_SIZE], PTR);
#define RELEASE_ARRAY(TYPE, NUMBER, PTR) giveback(&glob_alloc[(NUMBER*sizeof(TYPE))/CHUNK_SIZE], PTR);
#endif