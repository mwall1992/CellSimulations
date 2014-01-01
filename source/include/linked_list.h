#ifndef _linked_list_h
#define _linked_list_h

#include <stdbool.h>

typedef struct node node_t;

struct node {
	void* value;
	node_t* next;
};

node_t* linked_list_create();

void linked_list_destroy(node_t** list, bool const freeMemory);

void* linked_list_get(node_t* list, unsigned int index);

node_t* linked_list_push(node_t* list, void* value);

node_t* linked_list_delete(node_t* list, unsigned int index, 
	bool* deleted, bool const freeMemory);

node_t* linked_list_clear(node_t* list, bool const freeMemory);

unsigned int linked_list_size(node_t* list);

#endif