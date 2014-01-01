#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/lattice.h"
#include "include/lattice_parser.h"

int main(int argc, char* argv[]) {
	unsigned int rows = 3;
	unsigned int columns = 2;

	lattice_t* lattice = lattice_create(rows, columns, periodic, periodic,
		periodic, periodic);
	
	printf("After create: %p\n", lattice);

	coordinate_t coord;

	printf("-------------------------\n");

	printf("SINGLE AGENTS\n");


	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			int* value = malloc(sizeof(int));
			*value = i + j;
			lattice_push_agent(lattice, coord, value);
		}
	}

	printf("-------------------------\n");

	int* agentValue;
	unsigned int numAgents;
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			agentValue = (int*)lattice_get_agent(lattice, coord, 0);
			numAgents = lattice_get_agent_count(lattice, coord);
			printf("(%d, %d): %d, %p (count: %d)\n", i, j, *agentValue, 
				agentValue, numAgents);
		}
	}	

	printf("-------------------------\n");

	coord.row = 1;
	coord.column = 1;
	lattice_delete_agent(lattice, coord, 0);

	coord.row = 0;
	coord.column = 0;
	lattice_delete_agent(lattice, coord, 0);

	coord.row = 2;
	coord.column = 0;
	lattice_delete_agent(lattice, coord, 0);

	printf("-------------------------\n");

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			agentValue = (int*)lattice_get_agent(lattice, coord, 0);
			numAgents = lattice_get_agent_count(lattice, coord);

			if (agentValue) {
				printf("(%d, %d): %d, %p (count: %d)\n", i, j, *agentValue, 
					agentValue, numAgents);
			} else {
				printf("(%d, %d): NULL (count: %d)\n", i, j, numAgents);
			}
		}
	}
	
	printf("-------------------------\n");

	coord.row = 2;
	coord.column = 1;
	lattice_clear_agents(lattice, coord);

	printf("-------------------------\n");

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			agentValue = (int*)lattice_get_agent(lattice, coord, 0);
			numAgents = lattice_get_agent_count(lattice, coord);

			if (agentValue) {
				printf("(%d, %d): %d, %p (count: %d)\n", i, j, *agentValue, 
					agentValue, numAgents);
			} else {
				printf("(%d, %d): NULL (count: %d)\n", i, j, numAgents);
			}
		}
	}	

	printf("-------------------------\n");

	lattice_clear(lattice, rows, columns);

	printf("-------------------------\n");

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			agentValue = (int*)lattice_get_agent(lattice, coord, 0);
			numAgents = lattice_get_agent_count(lattice, coord);

			if (agentValue) {
				printf("(%d, %d): %d, %p (count: %d)\n", i, j, *agentValue, 
					agentValue, numAgents);
			} else {
				printf("(%d, %d): NULL (count: %d)\n", i, j, numAgents);
			}
		}
	}	

	printf("-------------------------\n");

	printf("MULTIPLE AGENTS\n");

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			int* value = malloc(sizeof(int));
			*value = i + j;
			int* anotherValue = malloc(sizeof(int));
			*anotherValue = i * j;
			lattice_push_agent(lattice, coord, value);
			lattice_push_agent(lattice, coord, anotherValue);
		}
	}

	printf("-------------------------\n");

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			agentValue = (int*)lattice_get_agent(lattice, coord, 1);
			numAgents = lattice_get_agent_count(lattice, coord);
			printf("(%d, %d): %d, %p (count: %d)\n", i, j, *agentValue, 
				agentValue, numAgents);
		}
	}	

	printf("-------------------------\n");

	coord.row = 1;
	coord.column = 1;
	lattice_delete_agent(lattice, coord, 0);

	coord.row = 0;
	coord.column = 0;
	lattice_delete_agent(lattice, coord, 1);

	coord.row = 2;
	coord.column = 0;
	lattice_delete_agent(lattice, coord, 1);

	printf("-------------------------\n");

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			agentValue = (int*)lattice_get_agent(lattice, coord, 0);
			numAgents = lattice_get_agent_count(lattice, coord);

			if (agentValue) {
				printf("(%d, %d): %d, %p (count: %d)\n", i, j, *agentValue, 
					agentValue, numAgents);
			} else {
				printf("(%d, %d): NULL (count: %d)\n", i, j, numAgents);
			}
		}
	}
	
	printf("-------------------------\n");

	coord.row = 2;
	coord.column = 1;
	lattice_clear_agents(lattice, coord);

	printf("-------------------------\n");

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			agentValue = (int*)lattice_get_agent(lattice, coord, 0);
			numAgents = lattice_get_agent_count(lattice, coord);

			if (agentValue) {
				printf("(%d, %d): %d, %p (count: %d)\n", i, j, *agentValue, 
					agentValue, numAgents);
			} else {
				printf("(%d, %d): NULL (count: %d)\n", i, j, numAgents);
			}
		}
	}	

	printf("-------------------------\n");

	bool parseResult = lattice_profile_parser(lattice, rows, columns, 
		"matrix.txt", "output/");
	if (!parseResult) {
		printf("******************\n");
		printf("FILE PARSER FAILED\n");
		printf("******************\n");
	} else {
		printf("******************\n");
		printf("FILE PARSER WORKED\n");
		printf("******************\n");
	}

	lattice_clear(lattice, rows, columns);

	printf("-------------------------\n");

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			agentValue = (int*)lattice_get_agent(lattice, coord, 0);
			numAgents = lattice_get_agent_count(lattice, coord);

			if (agentValue) {
				printf("(%d, %d): %d, %p (count: %d)\n", i, j, *agentValue, 
					agentValue, numAgents);
			} else {
				printf("(%d, %d): NULL (count: %d)\n", i, j, numAgents);
			}
		}
	}	

	printf("-------------------------\n");

	lattice_destroy(&lattice, rows, columns);

	if (lattice) {
		printf("After destroy: %p\n", lattice);
	} else {
		printf("After destroy: is NULL\n");
	}

	return EXIT_SUCCESS;
}
