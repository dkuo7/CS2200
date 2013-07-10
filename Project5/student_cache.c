#include "student_cache.h"
#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

int decode_offset(address_t address, student_cache_t *cache);
int decode_index(address_t address, student_cache_t *cache);
int decode_tag(address_t address, student_cache_t *cache);

int calc_obits(student_cache_t *cache);
int calc_ibits(student_cache_t *cache);
int calc_tbits(student_cache_t *cache);

cache_block* find_block(address_t addr, student_cache_t *cache, int write);
cache_block* find_invalid(address_t addr, student_cache_t *cache);
cache_block* find_LRU(address_t addr, student_cache_t *cache);
cache_block* get_block_from_way(int index, cache_way *way); 

void print_LRU(student_cache_t *cache, int block_index);

void set_used(student_cache_t *cache, int block_index, int way_index);


void transfer_from_memory(student_cache_t *cache, stat_t *stats);
void transfer_to_memory(student_cache_t *cache, stat_t *stats);

int two_power_of(int power);

int student_read(address_t addr, student_cache_t *cache, stat_t *stats){
    cache_block *block;
    stats->accesses++;
    stats->reads++;
    DEBUG_PRINT(("INDEX: %x\n",decode_index(addr,cache)));
    /* Check if in cache */
    block = find_block(addr,cache,0);
    if(block == NULL) {
        /* It's a miss so look for an invalid spot */
        block = find_invalid(addr,cache);
        if(block == NULL) {
            /* No invalids so evict the LRU */
            block = find_LRU(addr,cache);
            if(block->dirty && cache->WP == WBWA) {
                transfer_to_memory(cache,stats);
            }
        }
        /* Set up the block for the new address */
        block->dirty = 0;
        block->valid = 1;
        block->tag = decode_tag(addr,cache);
        stats->read_miss++;
        transfer_from_memory(cache,stats);
        return 0;
    }
    stats->read_hit++;
    return 1;
}

int student_write(address_t addr, student_cache_t *cache, stat_t *stats){
    cache_block *block;
    stats->accesses++;
    stats->writes++;
    if(cache->WP == WTWNA) {
        transfer_to_memory(cache,stats);
    }

    block = find_block(addr,cache,1);
    if(block == NULL) {
        if(cache->WP == WTWNA) {
            return 0;
        }
        /* It's a miss so look for an invalid spot */
        block = find_invalid(addr,cache);
        if(block == NULL) {
            /* No invalids so evict the LRU */
            block = find_LRU(addr,cache);
            if(block->dirty) {
                transfer_to_memory(cache,stats);
            }
        }
        /* Set up the block for the new address */
        block->valid = 1;
        block->dirty = 1;
        block->tag = decode_tag(addr,cache);
        stats->write_miss++;
        transfer_from_memory(cache,stats);
        return 0;
    }
    block->dirty = 1;
    stats->write_hit++;
    return 1;
}



void print_LRU(student_cache_t *cache, int block_index) {
    cache_LRU *current = cache->LRUs + block_index;
    while(current->next != NULL) {
        current = current->next;
    }
}

void set_used(student_cache_t *cache, int block_index, int way_index) {
    int hit = 0;
    cache_LRU *current = cache->LRUs + block_index;
    while(current->next != NULL) {
        if(current->way_index == way_index) {
            hit = 1;
        }
        if(hit) {
            current->way_index = current->next->way_index;
        }
        current = current->next;
    }
    current->way_index = way_index;
}

cache_block* find_invalid(address_t addr, student_cache_t *cache) {
    int i;
    int index = decode_index(addr,cache);
    cache_block *block;
    for(i=0; i<cache->ways_size; i++) {
        block = get_block_from_way(index,cache->ways+i);
        if(!block->valid) {
            set_used(cache,index,i); 
            return block;
        } 
    }
    return NULL;
}

cache_block* find_block(address_t addr, student_cache_t *cache, int write) {
    int i;
    int index = decode_index(addr,cache);
    int tag = decode_tag(addr,cache);
    cache_block *block;
    for(i=0; i<cache->ways_size; i++) {
       block = get_block_from_way(index,cache->ways+i);
       if(block->valid && block->tag == tag) {
           if(!write || cache->WP==WBWA) {
               set_used(cache,index,i); 
           }
           return block;
       }
    }
    return NULL;
}

cache_block* find_LRU(address_t addr, student_cache_t *cache) {
    int index = decode_index(addr,cache);
    cache_LRU *lru;
    cache_block *block;
    lru = cache->LRUs + index; 
    block = get_block_from_way(index,cache->ways + lru->way_index);
    set_used(cache,index,lru->way_index);
    return block;

}


cache_block* get_block_from_way(int index, cache_way *way) {
    return way->blocks + index;
}

void transfer_to_memory(student_cache_t *cache, stat_t* stats) {
    if(cache->WP==WBWA) {
        stats->mem_write_bytes += two_power_of(cache->B); 
    }
    else {
        stats->mem_write_bytes += 4;
    }
}

void transfer_from_memory(student_cache_t *cache, stat_t* stats) {
    stats->mem_read_bytes += two_power_of(cache->B);    
}

int decode_offset(address_t address, student_cache_t *cache) {
    int obits = calc_obits(cache);
    return address & ((1 << obits) - 1);
}

int decode_index(address_t address, student_cache_t *cache) {
    int obits = calc_obits(cache);
    int ibits = calc_ibits(cache);
    return (address & (((1 << ibits) - 1) << obits)) >> obits;
}

int decode_tag(address_t address, student_cache_t *cache) {
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
    /*cache_block *block;*/
    cache_LRU *lru;

    student_cache_t *cache = malloc(sizeof(student_cache_t));    

    cache->WP = WP;
    cache->C = C;
    cache->B = B;
    cache->S = S;
    cache->ways_size = two_power_of(S);
    cache->ways = calloc(cache->ways_size , sizeof(cache_way));
    cache->LRUs_size = two_power_of(C-B-S);
    cache->LRUs = calloc(cache->LRUs_size , sizeof(cache_LRU));

    for(i=0; i<cache->LRUs_size; i++) {
        lru = cache->LRUs + i;
        lru->way_index = 0;
        for(j=0; j<cache->ways_size; j++) {
           lru->way_index = j;
           if(j != cache->ways_size - 1) {
               lru->next = malloc(sizeof(cache_LRU));
               lru = lru->next;
           }
           else {
               lru->next = NULL;
           }
        }
    }

    for(i=0; i<cache->ways_size; i++) {
        way = cache->ways + i;
        way->blocks_size = two_power_of(C-B-S);
        way->blocks = calloc(way->blocks_size , sizeof(cache_block));
        /*for(j=0; j<way->blocks_size;j++) {
            block = way->blocks + j;
            block->valid = 0;
            block->dirty = 0;
            block->tag = 0;
        }*/
    }

    statistics->accesses = 0;
    statistics->misses = 0;
    statistics->reads = 0;
    statistics->writes = 0;
    statistics->read_hit = 0;
    statistics->write_hit = 0;
    statistics->read_miss = 0;
    statistics->write_miss = 0;
    statistics->mem_write_bytes = 0;
    statistics->mem_read_bytes = 0;
    statistics->read_hit_rate = 0;
    statistics->hit_rate = 0;
    statistics->total_bits = 0;
    statistics->AAT = 0;
	return cache;
}

int two_power_of(int power) {
    return 1 << power;
}

void finalize_stats(student_cache_t *cache, stat_t *statistics){
    int tag_size = calc_tbits(cache); 
    int block_meta = tag_size + 1;
    if(cache->WP == WBWA) {
        block_meta++;
    }
    statistics->total_bits += block_meta * two_power_of(cache->C - cache->B);
    int LRU_meta = two_power_of(cache->S) * cache->S;
    statistics->total_bits += LRU_meta * two_power_of(calc_ibits(cache));
    statistics->total_bits += two_power_of(cache->C) * 8;
    statistics->misses = statistics->read_miss + statistics->write_miss;
    statistics->read_hit_rate = (statistics->read_hit * 1.0)/(statistics->reads);
    statistics->hit_rate = (statistics->read_hit + statistics->write_hit)*1.0 / statistics->accesses;
    double hit_time = 2 + .2 * cache->S;
    double read_miss_rate = 1-statistics->read_hit_rate;
    statistics->AAT = hit_time + read_miss_rate * 50;
}


