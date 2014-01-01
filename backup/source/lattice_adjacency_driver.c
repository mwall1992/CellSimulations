#include <stdio.h>
#include <stdlib.h>
#include "include/lattice.h"

int main(int argc, char* argv[]) {
	unsigned int rows = 5;
	unsigned int columns = 5;

	printf("Horizontal: %d, Vertical: %d\n", horizontal, vertical);

	lattice_t* periodic_lattice = lattice_create(rows, columns, 
		periodic, periodic, periodic, periodic);

	lattice_t* reflective_lattice = lattice_create(rows, columns, 
		reflective, reflective, reflective, reflective);	

	coordinate_t adjacentCoord;
	coordinate_t anchor;
	int delta;
	orientation_t deltaOrientation;

	printf("--------------------------\n");

	anchor.row = 2;
	anchor.column = 0;
	delta = -6;
	deltaOrientation = horizontal;

	adjacentCoord = lattice_retrieved_adjacent_coord(periodic_lattice, rows, 
		columns, anchor, delta, deltaOrientation);

	printf("Anchor: (%d, %d), Adj: (%d, %d), Delta: %d, Orient: %d\n", 
		anchor.row, anchor.column, adjacentCoord.row, 
		adjacentCoord.column, delta, deltaOrientation);

	adjacentCoord = lattice_retrieved_adjacent_coord(reflective_lattice, rows, 
		columns, anchor, delta, deltaOrientation);

	printf("Anchor: (%d, %d), Adj: (%d, %d), Delta: %d, Orient: %d\n", 
		anchor.row, anchor.column, adjacentCoord.row, 
		adjacentCoord.column, delta, deltaOrientation);

	printf("--------------------------\n");

	anchor.row = 3;
	anchor.column = 4;
	delta = 6;
	deltaOrientation = horizontal;

	adjacentCoord = lattice_retrieved_adjacent_coord(periodic_lattice, rows, 
		columns, anchor, delta, deltaOrientation);

	printf("Anchor: (%d, %d), Adj: (%d, %d), Delta: %d, Orient: %d\n", 
		anchor.row, anchor.column, adjacentCoord.row, 
		adjacentCoord.column, delta, deltaOrientation);

	adjacentCoord = lattice_retrieved_adjacent_coord(reflective_lattice, rows, 
		columns, anchor, delta, deltaOrientation);

	printf("Anchor: (%d, %d), Adj: (%d, %d), Delta: %d, Orient: %d\n", 
		anchor.row, anchor.column, adjacentCoord.row, 
		adjacentCoord.column, delta, deltaOrientation);

	printf("--------------------------\n");

	anchor.row = 0;
	anchor.column = 1;
	delta = -6;
	deltaOrientation = vertical;

	adjacentCoord = lattice_retrieved_adjacent_coord(periodic_lattice, rows, 
		columns, anchor, delta, deltaOrientation);

	printf("Anchor: (%d, %d), Adj: (%d, %d), Delta: %d, Orient: %d\n", 
		anchor.row, anchor.column, adjacentCoord.row, 
		adjacentCoord.column, delta, deltaOrientation);

	adjacentCoord = lattice_retrieved_adjacent_coord(reflective_lattice, rows, 
		columns, anchor, delta, deltaOrientation);

	printf("Anchor: (%d, %d), Adj: (%d, %d), Delta: %d, Orient: %d\n", 
		anchor.row, anchor.column, adjacentCoord.row, 
		adjacentCoord.column, delta, deltaOrientation);

	printf("--------------------------\n");

	anchor.row = 4;
	anchor.column = 3;
	delta = 6;
	deltaOrientation = vertical;

	adjacentCoord = lattice_retrieved_adjacent_coord(periodic_lattice, rows, 
		columns, anchor, delta, deltaOrientation);

	printf("Anchor: (%d, %d), Adj: (%d, %d), Delta: %d, Orient: %d\n", 
		anchor.row, anchor.column, adjacentCoord.row, 
		adjacentCoord.column, delta, deltaOrientation);

	adjacentCoord = lattice_retrieved_adjacent_coord(reflective_lattice, rows, 
		columns, anchor, delta, deltaOrientation);

	printf("Anchor: (%d, %d), Adj: (%d, %d), Delta: %d, Orient: %d\n", 
		anchor.row, anchor.column, adjacentCoord.row, 
		adjacentCoord.column, delta, deltaOrientation);

	printf("--------------------------\n");

	return EXIT_SUCCESS;
}