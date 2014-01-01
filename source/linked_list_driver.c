#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/lattice.h"
#include "include/linked_list.h"

int main(int argc, char* argv[]) {
	
	int* test = malloc(sizeof(int));
	*test = 10;

	int* anotherTest = malloc(sizeof(int));
	*anotherTest = 12;

	int* lastTest = malloc(sizeof(int));
	*lastTest = 15;

	node_t* list = linked_list_create();
	printf("After create: %p\n", list);

	list = linked_list_push(list, test);
	list = linked_list_push(list, anotherTest);

	bool* deleted = malloc(sizeof(bool));

	printf("----------------------------\n");
	printf("%d, %p\n", *(int*)linked_list_get(list, 0), list->next);
	printf("%d, %p\n", *(int*)linked_list_get(list, 1), (list->next)->next);
	printf("%d\n", linked_list_size(list));
	printf("----------------------------\n");

	list = linked_list_delete(list, 0, deleted, false);

	printf("----------------------------\n");
	printf("%d, %p\n", *(int*)linked_list_get(list, 0), list->next);
	printf("%d\n", linked_list_size(list));
	printf("----------------------------\n");

	*anotherTest = 16;
	list = linked_list_push(list, anotherTest);

	printf("----------------------------\n");
	printf("%d, %p\n", *(int*)linked_list_get(list, 0), list->next);
	printf("%d, %p\n", *(int*)linked_list_get(list, 1), (list->next)->next);
	printf("%d\n", linked_list_size(list));
	printf("----------------------------\n");

	list = linked_list_delete(list, 1, deleted, false);

	printf("----------------------------\n");
	printf("%d, %p\n", *(int*)linked_list_get(list, 0), list->next);
	printf("%d\n", linked_list_size(list));
	printf("----------------------------\n");

	*test = 134;

	list = linked_list_push(list, test);
	list = linked_list_push(list, lastTest);

	printf("----------------------------\n");
	printf("%d, %p\n", *(int*)linked_list_get(list, 0), list->next);
	printf("%d, %p\n", *(int*)linked_list_get(list, 1), (list->next)->next);
	printf("%d, %p\n", *(int*)linked_list_get(list, 2), ((list->next)->next)->next);
	printf("%d\n", linked_list_size(list));
	printf("----------------------------\n");

	list = linked_list_delete(list, 1, deleted, false);

	printf("----------------------------\n");
	printf("%d, %p\n", *(int*)linked_list_get(list, 0), list->next);
	printf("%d, %p\n", *(int*)linked_list_get(list, 1), (list->next)->next);
	printf("%d\n", linked_list_size(list));
	printf("----------------------------\n");	

	list = linked_list_clear(list, false);

	printf("----------------------------\n");
	printf("%p, %p\n", linked_list_get(list, 0), list->next);
	printf("%d\n", linked_list_size(list));
	printf("----------------------------\n");

	*test = 2342;
	list = linked_list_push(list, test);

	printf("----------------------------\n");
	printf("%p, %p\n", linked_list_get(list, 0), list->next);
	printf("%d\n", linked_list_size(list));
	printf("----------------------------\n");

	linked_list_destroy(&list, true);

	if (list) {
		printf("After destroy: %p\n", list);
	} else {
		printf("After destroy: is NULL\n");
	}

	free(deleted);

	return 0;
}