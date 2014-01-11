#ifndef _lattice_parser_h
#define _lattice_parser_h

#include "lattice.h"
#include <stdbool.h>

bool lattice_profile_parser(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, char* filename, char* directory);

bool lattice_parser_node_positions(lattice_t* lattice, unsigned int rows,
	unsigned int columns, char* filename, char* directory);

bool tracked_agents_parser(coordinate_t** trackedAgentPositions, 
	unsigned int timeSteps, unsigned int trackedAgentIndex,
	 char* filename, char* directory);

#endif
