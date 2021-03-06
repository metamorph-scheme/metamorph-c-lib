#include "cache_internal.h"
#include "../cache.h"
#include <stddef.h>
#include <stdlib.h>

allocator_t glob_alloc[MAX_CHUNKS];
//DEBUG
int cbalance = 0;
int total = 0;
void initalize_allocator(){
    int i;
    for (i = 0; i < MAX_CHUNKS; i++)
        initalloc(&glob_alloc[i], i+1);
}

void* request(allocator_t* a){
    //DEBUG
    cbalance++;
    total++;
    
    //DELETE
#ifdef CACHE_OFF
    return calloc(a->chunk_count, CHUNK_SIZE);
#endif // CACHE_OFF

    if (a->index) {
        a->index--;
        return a->pointers[a->index];
    }
    return calloc(a->chunk_count, CHUNK_SIZE);
}

void giveback(allocator_t* a, void* ptr) {
    //DEBUG
    cbalance--;
    
    //DELETE
#ifdef CACHE_OFF
    free(ptr);
    return;
#endif // CHACHE_OFF



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
    a->pointers = calloc(sizeof(void*) , INIT_CACHE);
    a->chunk_count = size;
}