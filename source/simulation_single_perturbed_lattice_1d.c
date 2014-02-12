#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "include/simulation.h"
#include "include/lattice.h"
#include "include/lattice_parser.h"
#include "include/utilities.h"
#include <mkl.h>

#define BRNG VSL_BRNG_MCG31

/* Interface */

int compare(const void* a, const void* b);

/* Implementation */

int main(int argc, char* argv[]) {

	// construct lattice
	unsigned int rows = 1;
	unsigned int columns = 40;
	lattice_t* lattice = lattice_create(rows, columns, 
		periodic, periodic, periodic, periodic);

	// initialise lattice positioning
	unsigned int const kNumBounds = 5;
	double bounds[] = { 0.1, 0.2, 0.3, 0.4, 0.5 };

	// initialise random number storage
	VSLStreamStatePtr stream;
	float randomNumbers[columns];

	// initialise temporary node storage
	double xPosition;
	double yPosition = 0.0;
	coordinate_t coord;

	// initialise loop variables
	bool trackedLatticeLayout;
	char latticeLayoutFileName[50];
	char trackedAgentFileName[50];

	// initialise agent tracking information
	unsigned int numTrackedAgents;
	unsigned int timeSteps;
	int* trackedAgentIds;

	// set motility properties
	double motilityProbability = 1.0;
	double xShiftPreference = 0;
	double yShiftPreference = 0;
	bool agentExclusion = true;

	// generation random lattices
	for (int i = 0; i < kNumBounds; i++) {
		
		// generate any required random numbers (uniform dist)
		vslNewStream(&stream, BRNG, arc4random());	
		vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 
			columns, randomNumbers, -bounds[i], bounds[i]);

		// specify node locations
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				coord.row = i;
				coord.column = j;
				xPosition = (double)coord.column + randomNumbers[coord.column];
				lattice_specify_position(lattice, coord, xPosition, yPosition);
			}
		}

		// save node locations
		bool saveNodeLocations = true;
		if (saveNodeLocations) {
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

		// perform simulations
		unsigned int const kSimulationCount = 100;

		for (int j = 0; j < kSimulationCount; j++) {
			// populate lattice
			int* agentId = malloc(sizeof(int));
			*agentId = 1;

			coordinate_t agentPos = { .row = 0, .column = 19 };
			lattice_push_agent(lattice, agentPos, agentId);

			// set up tracking information
			numTrackedAgents = 1;
			timeSteps = 40;

			coordinate_t** trackedPositions =  malloc((timeSteps + 1) * sizeof(coordinate_t*));
			for (int i = 0; i < timeSteps + 1; i++) {
				trackedPositions[i] = malloc(numTrackedAgents * sizeof(coordinate_t));
			}

			trackedAgentIds = malloc(numTrackedAgents * sizeof(int));
			trackedAgentIds[0] = 1;
			trackedPositions[0][0] = agentPos;

			// perform simulation
			for (int i = 1; i < timeSteps + 1; i++) {
				performMotilityEvents(lattice, rows, columns, motilityProbability, 
					xShiftPreference, yShiftPreference, agentExclusion, 
					trackedAgentIds, numTrackedAgents, trackedPositions[i]);
			}

			// save tracking information
			sprintf(trackedAgentFileName, "tracked_agent_pos_%0.02f_%d.txt", 
				bounds[i], j + 1);
			bool isTracked =  tracked_agents_parser(trackedPositions, 
				timeSteps + 1, 0, trackedAgentFileName, "output/");
			if (isTracked) {
				printf("Stored tracking information (delta: %0.02f, case: %d).\n", 
					bounds[i], j + 1);
			} else {
				printf("Error: failed storing tracking information (delta: %0.02f, case: %d).\n", 
					bounds[i], j + 1);
			}

			// clear lattice and deallocate memory
			lattice_clear(lattice, rows, columns, true);
			for (int i = 0; i < timeSteps + 1; i++) {
				free(trackedPositions[i]);
			}
			free(trackedPositions);
		}
	}

	// deallocate memory
	lattice_destroy(&lattice, rows, columns, true);

	return EXIT_SUCCESS;
}

int compare(const void* a, const void* b) {
	int result = -1;
	float difference = *(float*)a - *(float*)b;

	if (difference >= 0) {
		result = 1;
	}
  	return result;
}
