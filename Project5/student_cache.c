#include "student_cache.h"


int student_read(address_t addr, student_cache_t *cache, stat_t *stats){
	return 0;
}

int student_write(address_t addr, student_cache_t *cache, stat_t *stats){
	return 0;
}

student_cache_t *allocate_cache(int C, int B, int S, int WP, stat_t* statistics){
    int i,j;
    cache_way *way;
    cache_block *block;

    student_cache_t *cache = malloc(sizeof(student_cache_t));    

    cache->ways_size = 2^S;
    cache->ways = calloc(cache->ways_size, sizeof(cache_way));

    for(i=0; i<cache->ways_size; i++) {
        way = cache->ways + i;
        way->blocks_size = 2^(C-B-S);
        way->blocks = calloc(way->blocks_size, sizeof(cache_block));

        for(j=0; j<way->blocks_size;j++) {
            block = way->blocks + i;
            block->valid = 0;
            block->dirty = 0;
            block->tag = 0;
        }
    }
	return cache;
}

void finalize_stats(student_cache_t *cache, stat_t *statistics){

}


