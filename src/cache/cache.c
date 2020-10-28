#include "cache.h"
#include "../cache.h"

allocator_t glob_alloc[MAX_CHUNKS];

void initalize_allocator(){
    int i;
    for (i = 0; i < MAX_CHUNKS; i++)
        initalloc(&glob_alloc[i], i+1);
}

void* request(allocator_t* a){
    if (a->index) {
        a->index--;
        return a->pointers[a->index];
    }
    return malloc(a->chunk_count * CHUNK_SIZE);
}

void giveback(allocator_t* a, void* ptr) {

    int c = a->current_size;
    int i = a->index;

    if (i == c) {
        void* tmp;
        if (tmp = realloc(a->pointers, (c * CACHE_GROW) * sizeof(void*))) {
            a->pointers = tmp;
            a->current_size *= CACHE_GROW;
            //a->current[size] += 1;
        }
        else {
            free(ptr);
        }
    }
    a->pointers[i] = ptr;
    a->index++;
    return;
}

void initalloc(allocator_t* a, size_t size) {
    a->current_size = INIT_CACHE;
    a->index = 0;
    a->pointers = malloc(sizeof(void*)* INIT_CACHE);
    a->chunk_count = size;
}