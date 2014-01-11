#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "include/simulation.h"
#include "include/lattice.h"
#include "include/lattice_parser.h"
#include "include/utilities.h"
#include <mkl.h>

#define BRNG VSL_BRNG_MCG31

int main(int argc, char* argv[]) {

	// construct lattice
	unsigned int rows = 1;
	unsigned int columns = 20;
	lattice_t* lattice = lattice_create(rows, columns, 
		periodic, periodic, periodic, periodic);

	// initialise random number storage
	VSLStreamStatePtr stream;
	float randomNumbers[columns];

	// initialise temporary node storage
	double xPosition;
	double yPosition = 0.0;
	coordinate_t coord;

	// initialise loop variables
	unsigned int const kRandomLattices = 5;
	bool trackedLatticeLayout;
	char latticeLayoutFileName[50];

	// generation random lattices
	for (int i = 0; i < kRandomLattices; i++) {
		
		// generate any required random numbers (uniform dist)
		vslNewStream(&stream, BRNG, arc4random());	
		vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 
			columns, randomNumbers, 0.0f, 20.0f);

		// specify node locations
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				coord.row = i;
				coord.column = j;
				xPosition = randomNumbers[coord.column];
				lattice_specify_position(lattice, coord, xPosition, yPosition);
			}
		}

		// save node locations
		sprintf(latticeLayoutFileName, "node_positions_rand_20_%d.txt", i + 1);
		trackedLatticeLayout = lattice_parser_node_positions(lattice, rows, 
			columns, latticeLayoutFileName, "output/");
		if (trackedLatticeLayout) {
			printf("Stored lattice layout information (case %d).\n", i + 1);
		} else {
			printf("Error: failed storing lattice layout information (case %d).\n", 
				i + 1);
		}
	}

	// deallocate memory
	lattice_destroy(&lattice, rows, columns, true);

	return EXIT_SUCCESS;
}