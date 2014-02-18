#include "lattice.h"
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>

/* Interface */

unsigned int lattice_determine_adjacent_coord_index(unsigned int index, int delta, 
	unsigned int maxIndex, boundary_t boundary);

/* Implementation */

lattice_t* lattice_create(unsigned int rows, unsigned int columns, 
	boundary_t lowerHorizontalBoundary, boundary_t upperHorizontalBoundary,
	boundary_t lowerVerticalBoundary, boundary_t upperVerticalBoundary) {
	
	lattice_t* lattice = (lattice_t*)malloc(sizeof(lattice_t));
	lattice->mesh = (point_t****)malloc(sizeof(point_t***));
	lattice->lowerHorizontalBoundary = lowerHorizontalBoundary;
	lattice->upperHorizontalBoundary = upperHorizontalBoundary;
	lattice->lowerVerticalBoundary = lowerVerticalBoundary;
	lattice->upperVerticalBoundary = upperVerticalBoundary;	

	*lattice->mesh = (point_t***)malloc(rows * sizeof(point_t**));
	
	for (int i = 0; i < rows; i++) {
		(*lattice->mesh)[i] = (point_t**)malloc(columns * sizeof(point_t*));
		for (int j = 0; j < columns; j++) {
			(*lattice->mesh)[i][j] = (point_t*)malloc(sizeof(point_t));
			(*lattice->mesh)[i][j]->numAgents = 0;
			(*lattice->mesh)[i][j]->agentList = linked_list_create();
			(*lattice->mesh)[i][j]->positionSpecified = false;
			(*lattice->mesh)[i][j]->x = 0.0;
			(*lattice->mesh)[i][j]->y = 0.0;
		}
	}

	return lattice;
}

void lattice_destroy(lattice_t** lattice, unsigned int rows, 
	unsigned int columns, bool const freeMemory) {

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			point_t* point = (*(*lattice)->mesh)[i][j];
			linked_list_destroy(&point->agentList, freeMemory);
			free(point);
		}
		free((*(*lattice)->mesh)[i]);
	}

	free(*(*lattice)->mesh);
	free(*lattice);
	*lattice = NULL;
}

void lattice_clear(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, bool const freeMemory) {

	coordinate_t coord;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			lattice_clear_agents(lattice, coord, freeMemory);
		}
	}
}

int* lattice_get_agent(lattice_t* lattice, coordinate_t coord,
	unsigned int index) {

	point_t* point = (*lattice->mesh)[coord.row][coord.column];
	return linked_list_get(point->agentList, index);
}

void lattice_specify_position(lattice_t* lattice, coordinate_t coord,
	double x, double y) {

	point_t* point = (*lattice->mesh)[coord.row][coord.column];
	point->positionSpecified = true;
	point->x = x;
	point->y = y;
}

void lattice_remove_specified_position(lattice_t* lattice, coordinate_t coord) {
	point_t* point = (*lattice->mesh)[coord.row][coord.column];
	point->positionSpecified = false;
	point->x = 0.0;
	point->y = 0.0;
}

bool lattice_get_specified_position(lattice_t* lattice, coordinate_t coord,
	double* x, double* y) {

	point_t* point = (*lattice->mesh)[coord.row][coord.column];
	bool positionSpecified = point->positionSpecified;

	if (positionSpecified) {
		*x = point->x;
		*y = point->y;
	}
	return positionSpecified;
}

unsigned int lattice_get_agent_count(lattice_t* lattice, 
	coordinate_t coord) {

	point_t* point = (*lattice->mesh)[coord.row][coord.column];
	return linked_list_size(point->agentList);
	// return point->numAgents;
}

unsigned int lattice_get_total_agent_count(lattice_t* lattice, unsigned int rows, 
	unsigned int columns) {
	unsigned int total = 0;

	coordinate_t coord;
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			coord.row = i;
			coord.column = j;
			total += lattice_get_agent_count(lattice, coord);
		}
	}

	return total;
}

void lattice_push_agent(lattice_t* lattice, coordinate_t coord, 
	int* value) {

	point_t* point = (*lattice->mesh)[coord.row][coord.column];
	point->agentList = linked_list_push(point->agentList, value);
	point->numAgents++;
}

void lattice_delete_agent(lattice_t* lattice, coordinate_t coord, 
	unsigned int index, bool const freeMemory) {

	bool* deleted = (bool*)malloc(sizeof(bool));

	point_t* point = (*lattice->mesh)[coord.row][coord.column];
	point->agentList = linked_list_delete(point->agentList, index, deleted, 
		freeMemory);

	if (*deleted) {
		point->numAgents--;
	} else {
		printf("Nothing deleted!\n");
	}

	free(deleted);
}

void lattice_move_agent(lattice_t* lattice, coordinate_t oldPosition,
	unsigned int oldIndex, coordinate_t newPosition) {

	bool const kFreeAgentMemory = false;

	void* value = lattice_get_agent(lattice, oldPosition, oldIndex);
	lattice_push_agent(lattice, newPosition, value);
	lattice_delete_agent(lattice, oldPosition, oldIndex, kFreeAgentMemory);
}

void lattice_clear_agents(lattice_t* lattice, coordinate_t coord, 
	bool const freeMemory) {
	point_t* point = (*lattice->mesh)[coord.row][coord.column];
	point->agentList = linked_list_clear(point->agentList, freeMemory);
	point->numAgents = 0;
}

coordinate_t lattice_retrieved_adjacent_coord(lattice_t* lattice, 
	unsigned int rows, unsigned int columns, coordinate_t anchor,
	int delta, orientation_t deltaOrientation) {

	if (!delta) {
		return anchor;
	}

	int const kCoordToIndexDelta = -1;

	coordinate_t adjacentCoord;
	boundary_t applicableBounary;
	bool effectsRow;

	switch (deltaOrientation) {
		case horizontal:
			applicableBounary = (delta > 0) ? lattice->upperHorizontalBoundary : 
				lattice->lowerHorizontalBoundary;
			effectsRow = false;
			break;

		case vertical:
			applicableBounary = (delta > 0) ? lattice->lowerVerticalBoundary : 
				lattice->upperVerticalBoundary;
			effectsRow = true;
			break;
	}

	if (effectsRow) {
		adjacentCoord.column = anchor.column;
		adjacentCoord.row = lattice_determine_adjacent_coord_index(anchor.row, 
			delta, rows, applicableBounary);
	} else {
		adjacentCoord.row = anchor.row;
		adjacentCoord.column = 
			lattice_determine_adjacent_coord_index(anchor.column, delta, 
				columns, applicableBounary);
	}

	return adjacentCoord;
}

unsigned int lattice_determine_adjacent_coord_index(unsigned int index, 
	int delta, unsigned int dimensions, boundary_t boundary) {

	unsigned int maxIndex = dimensions - 1;
	unsigned int adjacentIndex;
	int proposedIndex = (index + delta);

	if (abs(delta) == dimensions) {
		return index;
	}

	if (proposedIndex >= (int)dimensions) {
		int overlap = (proposedIndex - maxIndex) % dimensions;
		if (boundary == periodic) {
			adjacentIndex = overlap - 1;
		} else if (boundary == reflective) {
			adjacentIndex = maxIndex - overlap;
		}

	} else if (proposedIndex < 0) {
		int underlap = abs(proposedIndex) % dimensions;
		if (boundary == periodic) {
			adjacentIndex = dimensions - underlap % dimensions;
		} else if (boundary == reflective) {
			adjacentIndex = underlap;
		}

	} else {
		adjacentIndex = proposedIndex;
	}

	return adjacentIndex;
}

coordinate_t lattice_retrieve_agent_coord(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, unsigned int orderedPosition, unsigned int* agentIndex) {

	if (orderedPosition > lattice_get_total_agent_count(lattice, rows, columns)) {
		printf("Error: attempted to retrieve non-existent agent.\n");
		coordinate_t notFoundCoord = { .row = rows + 1, .column = columns + 1 };
		return notFoundCoord;
	}

	unsigned int nextPositionReached = 0;
	unsigned int positionReached = 0;

	bool found = false;
	coordinate_t currentCoord;

	for (int i = 0; i < rows && !found; i++) {
		for (int j = 0; j < columns && !found; j++) {
			currentCoord.row = i;
			currentCoord.column = j;
			nextPositionReached += lattice_get_agent_count(lattice, currentCoord);

			if (nextPositionReached >= orderedPosition) {
				*agentIndex =  orderedPosition - positionReached - 1;
				found = true;
			} else {
				positionReached = nextPositionReached;
			}
		}
	}

	return currentCoord;
}

coordinate_t lattice_find_agent(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, int* value) {

	coordinate_t agentCoord = { .row = rows + 1, .column = columns + 1 };

	int* agentValue;
	int agentCount;
	coordinate_t coord;

	bool found = false;

	for (int i = 0; i < rows && !found; i++) {
		for (int j = 0; j < columns && !found; j++) {
			coord.row = i;
			coord.column = j;
			agentCount = lattice_get_agent_count(lattice, coord);

			for (int k = 0; k < agentCount && !found; k++) {
				agentValue = lattice_get_agent(lattice, coord, k);

				if (*agentValue == *value) {
					found = true;
					agentCoord = coord;
				}
			}
		}
	}

	return agentCoord;
}
