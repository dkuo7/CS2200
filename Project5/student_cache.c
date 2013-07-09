#include "student_cache.h"

int decode_offset(unsigned int address, student_cache_t *cache);
int decode_index(unsigned int address, student_cache_t *cache);
int decode_tag(unsigned int address, student_cache_t *cache);

int calc_obits(student_cache_t *cache);
int calc_ibits(student_cache_t *cache);
int calc_tbits(student_cache_t *cache);

int student_read(address_t addr, student_cache_t *cache, stat_t *stats){
    int i;
    cache_way *way;
    cache_block *block;

    for(i=0; i<cache->ways_size; i++) {
       way = cache->ways + i;
       block = way->blocks + decode_index(addr,cache);
    }
    return 0;
}

int student_write(address_t addr, student_cache_t *cache, stat_t *stats){
	return 0;
}

int decode_offset(unsigned int address, student_cache_t *cache) {
    int obits = calc_obits(cache);
    return address & ((1 << obits) - 1);
}

int decode_index(unsigned int address, student_cache_t *cache) {
    int obits = calc_obits(cache);
    int ibits = calc_ibits(cache);
    return (address & (((1 << ibits) - 1) << obits)) >> obits;
}

int decode_tag(unsigned int address, student_cache_t *cache) {
    int obits = calc_obits(cache);
    int ibits = calc_ibits(cache);
    int tbits = calc_tbits(cache);
    return (address & (((1 << tbits) - 1) << obits << ibits)) >> obits >> ibits; 
}

int calc_obits(student_cache_t *cache) {
    return cache->B; 
}

int calc_ibits(student_cache_t *cache) {
    return cache->C - cache->B - cache-> S;
}

int calc_tbits(student_cache_t *cache) {
    return 32 - calc_obits(cache) - calc_ibits(cache);
}
student_cache_t *allocate_cache(int C, int B, int S, int WP, stat_t* statistics){
    int i,j;
    cache_way *way;
    cache_block *block;

    student_cache_t *cache = malloc(sizeof(student_cache_t));    

    cache->WP = WP;
    cache->C = C;
    cache->B = B;
    cache->S = S;
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


