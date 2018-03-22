#include "hw_malloc.h"

struct chunk_header *pre=NULL;
struct chunk_header che;

void *hw_malloc(size_t bytes)
{
	int a = 0;
	a = (bytes / 8) - 1;
	if(a>6) {
		a = 6;
	}

	int qq = 0;
	if (!pre) {
//		printf("create heap\n");
		pre = sbrk(65536);
		void *now = pre;
		pre->chunk_size = bytes + 40;
		pre->prev_free_flag = 0;
		pre->pre_chunk_size = 0;
		pre->entry.tqe_next = &che;

		now = now + pre->chunk_size;
		struct chunk_header *new = now;
		new->chunk_size = 0;
		new->prev_free_flag=0;
		new->entry.tqe_next = &che;

		return pre + 1;
	} else if (!TAILQ_EMPTY(&bin[a]))  {

//		printf("split a\n");
//		fflush(stdout);

		struct chunk_header *abc = TAILQ_FIRST(&bin[a]);
		TAILQ_REMOVE(&bin[a],abc,entry);
		abc->entry.tqe_next = &che;
		void *tru = abc + bytes + 40;
		struct chunk_header *abcn = tru;
		abcn->prev_free_flag = 0;
		return abc+1;
	} else {
		fflush(stdout);
		struct chunk_header *search = malloc(sizeof(struct chunk_header));
		TAILQ_FOREACH(search,&bin[6],entry) {
			if(search->chunk_size - bytes > 48) {
				/*
								printf("split 6\n");
								fflush(stdout);

				*/
				TAILQ_REMOVE(&bin[6],search,entry);
				struct chunk_header *abc = search;
				void *tru = abc;
				tru += bytes + 40;
				struct chunk_header *abcn = tru;

				abcn->prev_free_flag = 0;
				abcn->chunk_size = search->chunk_size - 40 - bytes;
				abcn->pre_chunk_size = 40 + bytes;
				abc->chunk_size = 40 + bytes;
				abc->entry.tqe_next = &che;
				int g = ((abcn->chunk_size - 40) / 8) - 1;
				if (g > 6) {
					g = 6;
				}

				TAILQ_INSERT_TAIL(&bin[g], abcn, entry);

				return abc + 1;
			}
		}

//		printf("new create\n");
//		fflush(stdout);

		struct chunk_header *now = pre;
		struct chunk_header *prev;

		while (now->chunk_size>0) {
			prev = now;
			void *new = now;
			new += now->chunk_size;
			now = new;
		}

		struct chunk_header *create;
		create = now;
		create->chunk_size=bytes+40;
		create->pre_chunk_size = prev->chunk_size;
		create->entry.tqe_next = &che;

		void *nnn = create;
		nnn += create->chunk_size;
		struct chunk_header *nnnn = nnn;

		nnnn->chunk_size = 0;

		return create + 1;

	}
}


int hw_free(void *mem)
{

//	printf("m=0x%08lx\n",mem);
	void **check = mem - 40;
	if (*check!=&che) {
		return 0;
	}

	struct chunk_header *cfree = (struct chunk_header *)mem - 1;
	size_t r = cfree->chunk_size;

	int a = ((cfree->chunk_size - 40) / 8) - 1;
	if (a > 6) {
		a = 6;
	}

	TAILQ_INSERT_TAIL(&bin[a], cfree, entry);

	void *nex = cfree;
	nex += cfree->chunk_size;
	struct chunk_header *next = nex;
	next->prev_free_flag = 1;
	if(cfree->prev_free_flag) {
		void *sprev = cfree;
		sprev -= cfree->pre_chunk_size;
		struct chunk_header *cprev = sprev;
		int c = ((cprev->chunk_size - 40) / 8) - 1;
		if (c > 6) {
			c = 6;
		}

		TAILQ_REMOVE(&bin[c],cprev,entry);
		TAILQ_REMOVE(&bin[a],cfree,entry);

		cprev->chunk_size += cfree->chunk_size;
		next->pre_chunk_size += cfree->chunk_size;

		int d = ((cprev->chunk_size - 40) / 8) - 1;
		if (d > 6) {
			d = 6;
		}
		TAILQ_INSERT_TAIL(&bin[d], cprev, entry);
		cfree=cprev;
		nex = cfree;
		nex += cfree->chunk_size;
		next = nex;
//			printf("merge\n");
	}

	if(next->chunk_size>0) {

//	printf("aa\n");
//	fflush(stdout);
		next->prev_free_flag = 1;
		void *nnex = next;
		nnex += next->chunk_size;
		struct chunk_header *nnext = nnex;

		if (nnext->prev_free_flag) {
			int b = ((nnext->chunk_size - 40) / 8) - 1;
			if (b > 6) {
				b = 6;
				TAILQ_REMOVE(&bin[b],nnext,entry);
				nnext->pre_chunk_size = cfree->chunk_size+next->chunk_size;
				cfree->chunk_size = nnext->pre_chunk_size;
//			printf("merge\n");

			}
		}
	}



	return r;
}

void *get_start_sbrk(void)
{
	return pre;
}
