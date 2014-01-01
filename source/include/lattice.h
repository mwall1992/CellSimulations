#ifndef _lattice_h
#define _lattice_h

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
	unsigned int columns);

void lattice_clear(lattice_t* lattice, unsigned int rows, 
	unsigned int columns);

void* lattice_get_agent(lattice_t* lattice, coordinate_t coord,
	unsigned int index);

unsigned int lattice_get_total_agent_count(lattice_t* lattice, unsigned int rows, 
	unsigned int columns);

unsigned int lattice_get_agent_count(lattice_t* lattice, 
	coordinate_t coord);

void lattice_push_agent(lattice_t* lattice, coordinate_t coord, 
	void* value);

void lattice_delete_agent(lattice_t* lattice, coordinate_t coord, 
	unsigned int index);

void lattice_move_agent(lattice_t* lattice, coordinate_t oldPosition,
	unsigned int oldIndex, coordinate_t newPosition);

void lattice_clear_agents(lattice_t* lattice, coordinate_t coord);

coordinate_t retrieve_adjacent_coord(lattice_t* lattice, 
	unsigned int rows, unsigned int columns, coordinate_t anchor,
	int delta, orientation_t deltaOrientation);

coordinate_t retrieve_agent_coord(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, unsigned int orderedPosition, unsigned int* agentIndex);

coordinate_t find_agent(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, void* value);

#endif
