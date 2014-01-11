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

	// initialise lattice positioning
	unsigned int const kNumBounds = 12;
	double bounds[] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 2.0, 3.0 };

	// initialise random number storage
	float randomNumbers[columns];
	VSLStreamStatePtr stream;
	
	// initialise temporary node storage
	double xPosition;
	double yPosition = 0.0;
	coordinate_t coord;

	// initialise loop parameters
	bool trackedLatticeLayout;
	char latticeLayoutFileName[50];

	// generate lattice node positions
	for (int i = 0; i < kNumBounds; i++) {

		// generate any required random numbers (uniform dist)
		vslNewStream(&stream, BRNG, arc4random());	
		vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 
			columns, randomNumbers, -bounds[i], bounds[i]);

		// place nodes
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				coord.row = i;
				coord.column = j;
				xPosition = (double)coord.column + randomNumbers[coord.column];
				lattice_specify_position(lattice, coord, xPosition, yPosition);
			}
		}

		// save node locations
		sprintf(latticeLayoutFileName, "node_positions_%0.02f.txt", bounds[i]);
		trackedLatticeLayout = lattice_parser_node_positions(lattice, rows, 
			columns, latticeLayoutFileName, "output/");
		if (trackedLatticeLayout) {
			printf("Stored lattice layout information (delta: %0.02f).\n", bounds[i]);
		} else {
			printf("Error: failed storing lattice layout information (delta: %0.02f).\n", 
				bounds[i]);
		}
	}
	
	// deallocate memory
	lattice_destroy(&lattice, rows, columns, true);

	return EXIT_SUCCESS;
}