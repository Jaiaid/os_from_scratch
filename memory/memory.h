#include<stdint.h>

#define MEMPAGE 8192

typedef struct{
	uint32_t *content;
} pagetable_entry;

typedef struct{
	pagetable_entry *addrs;
} pagedir_entry;

