#include <getopt.h>
#include "student_cache.h"

#define WP_DEFAULT WTWNA
#define C_DEFAULT 10
#define S_DEFAULT 0
#define B_DEFAULT 2


void print_stats(stat_t* stats);

int main(int argc, char* argv[]){
	int C = C_DEFAULT, B = B_DEFAULT, S = S_DEFAULT, WP = WP_DEFAULT;
	int opt; 
	stat_t stats;
	while((opt = getopt(argc, argv, "c:C:b:B:s:S:pP?"))!=-1){
		switch(opt){
		case 'c':
		case 'C':
			C = atoi(optarg);
			break;
		case 'b':
		case 'B':
			B = atoi(optarg);
			break;
		case 's':
		case 'S':
			S = atoi(optarg);
			break;
		case 'p':
		case 'P':
			WP = WBWA;
			break;
		case '?':
			printf("Usage: %s <options>\n", argv[0]);
			printf("\tOption: -c <value>\tset C to <value>\n");
			printf("\tOption: -b <value>\tset B to <value>\n");
			printf("\tOption: -s <value>\tset S to <value>\n");
			printf("\tOption: -p        \ttoggle write policy to WBWA\n");
			printf("\tOption: -?        \tprint this message and exit\n");
			return 0;
		}
	}
	printf("Cache: (%d, %d, %d) %s\n", C,B,S,WP==WBWA?"WBWA":"WTWNA");
	student_cache_t *cache = allocate_cache(C, B, S, WP, &stats);
	char rw;
	address_t addr;
	while(2 == scanf("%c %x\n", &rw, &addr)){
		switch(rw){
		case 'r':
			printf("Read %s for address %08x\n", student_read(addr, cache, &stats)?"HIT":"MISS",addr);
			break;
		case 'w':
			printf("Write %s for address %08x\n", student_write(addr, cache, &stats)?"HIT":"MISS",addr);
			break;
		}
	}
	finalize_stats(cache, &stats);
	print_stats(&stats);
	return 0;
}


void print_stats(stat_t* stats){
	printf("Total memory/cache accesses: %ld\n", stats->accesses);       //1
	printf("Total reads: %ld\n", stats->reads);                          //2
	printf("Total read misses: %ld\n", stats->read_miss);                //3
	printf("Total read hits: %ld\n", stats->read_hit);                   //3.5
	printf("Total writes: %ld\n", stats->writes);                        //4
	printf("Total write misses: %ld\n", stats->write_miss);              //5
	printf("Total write hits: %ld\n", stats->write_hit);                 //5.5
	printf("Bytes transferred to memory: %ld\n", stats->mem_write_bytes);//6
	printf("Bytes transfered from memory: %ld\n", stats->mem_read_bytes);//6.5
	printf("Total cache misses: %ld\n", stats->misses);                  //7
	printf("Read hit rate: %.5lf\n", stats->read_hit_rate);              //8
	printf("Hit rate: %.5lf\n", stats->hit_rate);                        //8.5
	printf("Total cache size (bits): %ld\n", stats->total_bits);         //9
	printf("AAT: %.5lf\n", stats->AAT);                                  //AAT
}
