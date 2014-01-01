#include "linked_list.h"
#include <stdlib.h>

node_t* linked_list_create() {
	node_t* list = (node_t*)malloc(sizeof(node_t));
	list->value = NULL;
	list->next = NULL;
	return list;
}

void linked_list_destroy(node_t** list) {
	*list = linked_list_clear(*list);
	free(*list);
	*list = NULL;
}

void* linked_list_get(node_t* list, unsigned int index) {
	unsigned int currentIndex = index;
	node_t* currentNode = list;
	
	if (!currentNode->value && !currentNode->next) {
		return NULL;
	}

	while (currentIndex) {
		if (!currentIndex) {
			return NULL;
		}

		currentNode = currentNode->next;
		currentIndex--;
	}

	return currentNode->value;
}

node_t* linked_list_push(node_t* list, void* value) {
	if (!list->value) {
		list->value = value;
	} else {
		node_t* oldHead = linked_list_create();
		oldHead->value = list->value;
		oldHead->next = list->next;
		list->value = value;
		list->next = oldHead;
	}

	return list;
}

node_t* linked_list_delete(node_t* list, unsigned int index, bool* deleted) {
	*deleted  = true;
	unsigned int listSize = linked_list_size(list);

	if (listSize && listSize > index) {

		node_t* currentParent = NULL;
		node_t* currentNode = list;
		for (int i = 0; i < index; i++) {
			currentParent = currentNode;
			currentNode = currentNode->next;
		}

		if (currentParent) {
			list = currentParent;
			currentParent->next = currentNode->next;
			//free(currentNode->value);
			free(currentNode);
			currentNode = NULL;
		} else {

			if (!currentNode->next) {
				list = currentNode;
				list->value = NULL;
			} else {
				list = currentNode->next;
			}
		}
	} else {
		*deleted = false;
	}

	return list;
}

node_t* linked_list_clear(node_t* list) {
	node_t* oldHead = NULL;
	node_t* currentHead = list;

	while (currentHead->next) {
		oldHead = currentHead;
		currentHead = currentHead->next;
		//free(oldHead->value);
		free(oldHead);
		oldHead = NULL;
	}
	currentHead->value = NULL;

	return currentHead;
}

unsigned int linked_list_size(node_t* list) {
	unsigned int size = 0;
	node_t* currentNode = list;

	if (!currentNode->value && !currentNode->next) {
		return 0;
	}

	while (currentNode) {
		size++;
		currentNode = currentNode->next;
	}
	return size;
}
