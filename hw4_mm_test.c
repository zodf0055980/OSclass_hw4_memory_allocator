#include "hw4_mm_test.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/queue.h>

int main()
{
	int i;
	for(i=0; i<7; i++) {
		TAILQ_INIT(&bin[i]);
	}

	char *get_f = malloc(20*sizeof(char));
	char *get_s = malloc(20*sizeof(char));

	printf("chunk_header size: %ld\n", sizeof(struct chunk_header));

	while(scanf("%s %s",get_f,get_s)!=EOF) {
		if(strcmp(get_f,"alloc")==0) {
//			printf("start_brk: %p\n", get_start_sbrk());
			size_t byte =0;
			sscanf(get_s,"%d",&byte);
			if(byte % 8 ==0) {
				printf("0x%08lx\n", hw_malloc(byte) - get_start_sbrk());
			}
		} else if(strcmp(get_f,"free")==0) {
			int memorysize =0;
			sscanf(get_s,"%x",&memorysize);
			int gf = hw_free(memorysize + get_start_sbrk());
			printf("%s--------%d\n", gf > 0 ? "success" : "fail",gf);
		} else if(strcmp(get_f,"print")==0) {
			int key=atoi(&get_s[4]);

//			printf("start_brk: %p\n", get_start_sbrk());
			struct chunk_header *search = malloc(sizeof(struct chunk_header));
			TAILQ_FOREACH(search,&bin[key],entry) {
				void *a = search;
				printf("0x%08lx\n", a - get_start_sbrk());
			}
		} else {
			printf("start_brk: %p\n", get_start_sbrk());
		}
	}
	/*
	    printf("chunk_header size: %ld\n", sizeof(struct chunk_header));
	    printf("%p\n", hw_malloc(8));
	    printf("%s\n", hw_free(NULL) == 1 ? "success" : "fail");
	    printf("start_brk: %p\n", get_start_sbrk());
	  */  return 0;
}
