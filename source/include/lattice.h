#ifndef _lattice_h
#define _lattice_h

#include <stdbool.h>
#include "linked_list.h"

typedef enum boundary boundary_t;

enum boundary {
	periodic,
	reflective
};

typedef enum orientation orientation_t;

enum orientation {
	horizontal,
	vertical
};

typedef struct coordinate coordinate_t;

struct coordinate {
	unsigned int row;
	unsigned int column;
};

typedef struct point point_t;

struct point {
	unsigned int numAgents;
	node_t* agentList;
	double x;
	double y;
	bool positionSpecified;
};

typedef struct lattice lattice_t;

struct lattice {
	point_t**** mesh;
	boundary_t lowerHorizontalBoundary;
	boundary_t upperHorizontalBoundary;
	boundary_t lowerVerticalBoundary;
	boundary_t upperVerticalBoundary;
};

lattice_t* lattice_create(unsigned int rows, unsigned int columns, 
	boundary_t lowerHorizontalBoundary, boundary_t upperHorizontalBoundary,
	boundary_t lowerVerticalBoundary, boundary_t upperVerticalBoundary);

void lattice_destroy(lattice_t** lattice, unsigned int rows, 
	unsigned int columns, bool const freeMemory);

void lattice_clear(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, bool const freeMemory);

int* lattice_get_agent(lattice_t* lattice, coordinate_t coord,
	unsigned int index);

void lattice_specify_position(lattice_t* lattice, coordinate_t coord,
	double x, double y);

void lattice_remove_specified_position(lattice_t* lattice, coordinate_t coord);

bool lattice_get_specified_position(lattice_t* lattice, coordinate_t coord,
	double* x, double* y);

unsigned int lattice_get_total_agent_count(lattice_t* lattice, unsigned int rows, 
	unsigned int columns);

unsigned int lattice_get_agent_count(lattice_t* lattice, 
	coordinate_t coord);

void lattice_push_agent(lattice_t* lattice, coordinate_t coord, 
	int* value);

void lattice_delete_agent(lattice_t* lattice, coordinate_t coord, 
	unsigned int index, bool const freeMemory);

void lattice_move_agent(lattice_t* lattice, coordinate_t oldPosition,
	unsigned int oldIndex, coordinate_t newPosition);

void lattice_clear_agents(lattice_t* lattice, coordinate_t coord, 
	bool const freeMemory);

coordinate_t lattice_retrieved_adjacent_coord(lattice_t* lattice, 
	unsigned int rows, unsigned int columns, coordinate_t anchor,
	int delta, orientation_t deltaOrientation);

coordinate_t lattice_retrieve_agent_coord(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, unsigned int orderedPosition, unsigned int* agentIndex);

coordinate_t lattice_find_agent(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, int* value);

#endif
