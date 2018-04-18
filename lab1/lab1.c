#include <stdio.h>

#include <string.h>

#include <malloc.h>

#include <stdlib.h>

#define maxsize 1000  //定义原始分配区的大小。

struct map {
   unsigned m_size;
   char *m_addr;
   struct map *next, *prior;
};
struct map *coremap;

char *lmalloc(unsigned size) {
    struct map *cur = coremap;
    do {
        if(cur->m_size > size) {    // 空闲区大于要分配区域
            cur->m_size -= size;
            cur->m_addr += size;
            return cur->m_addr;
        } else if(cur->m_size == size) {    // 空闲区大于要分配区域,删除空闲区
            cur->m_size = 0;
            cur->prior->next = cur->next;
            cur->next->prior = cur->prior;
            return cur->m_addr;
        } else
            cur = cur->next;
    } while(cur != coremap);

    return NULL;

}

int lfree(unsigned size, char * addr){
    struct map *cur = coremap;

	if(coremap->m_addr > addr + size) {
		struct map *temp = (struct map * )malloc(sizeof (struct map));
		temp->m_size = size;
		temp->m_addr = addr;
		temp->next = coremap;
		temp->prior = coremap->prior;
		coremap->prior->next = temp;
		coremap->prior = temp;
		coremap = temp;
		return 1;
	} else if(coremap->m_addr == addr + size) {
		coremap->m_addr = addr;
		coremap->m_size += size;
		return 1;
	}

    do {
        if(cur->m_addr + cur->m_size < addr && cur->next->m_addr > addr + size) {
            struct map *temp = cur->next;
            cur->next=(struct map * )malloc(sizeof (struct map));
            cur->next->m_addr = addr;
            cur->next->m_size = size;
            cur->next->prior = cur;
            cur->next->next = temp;
            temp->prior = cur->next;
            return 1;
        } else if(cur->m_addr + cur->m_size < addr && cur->next->m_addr == addr + size) {
            cur->next->m_addr = addr;
            cur->next->m_size += size;
            return 1;
        } else if(cur->m_addr + cur->m_size == addr && cur->next->m_addr > addr + size) {
            cur->m_size += size;
            return 1;
        } else if(cur->m_addr + cur->m_size == addr && cur->next->m_addr == addr + size) {
            cur->m_size += size + cur->next->m_size;
            cur->next = cur->next->next;
            cur->next->next->prior = cur;
            return 1;
        } else
            cur = cur->next;
    } while(cur != coremap);

	return 0;

}

void printTable() {
	struct map *cur = coremap;
	printf("Free storage area table:\n");
	do {
		printf("%u,%u;",cur->m_addr,cur->m_size);
		cur = cur->next;
	} while(cur != coremap);
}

int main() {
	
	coremap = (struct map *) malloc(sizeof(struct map));
	coremap->m_addr = (char *) malloc(maxsize);
	coremap->m_size = maxsize;
	coremap->next = coremap;
	coremap->prior = coremap;

	unsigned size;
	char behavior;
	char *addr;

	do {
		printTable();
		printf("\n");
		printf("Do you want to malloc or free? Type in m(malloc) or f(free). \n");
		scanf("%s", &behavior);
		if(behavior == 'm') {
			scanf("%u", &size);
			if(!lmalloc(size))
				printf("There's no enough space to be allocated.\n");
		} else if(behavior == 'f') {
			scanf("%u,%u", &size, &addr);
			if(!lfree(size, addr))
				printf("There's no area you can free.\n");
		} else {
			printf("Please enter valid character as instructed.\n");
			break;
		}
	} while(behavior == 'm' || behavior == 'f');

}
