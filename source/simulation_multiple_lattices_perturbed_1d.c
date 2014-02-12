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
	unsigned int timeSteps = 40;
	unsigned int const kRandomLattices = 100;
	char nodeLocationsFilename[50];
	char agentLocationsFilename[50];

	// initialise agent tracking information
	unsigned int numTrackedAgents;
	int* trackedAgentIds;

	// set motility properties
	double motilityProbability = 1.0;
	double xShiftPreference = 0;
	double yShiftPreference = 0;
	bool agentExclusion = true;

	// initialise lattice tracking information
	double** nodeLocations = malloc(kRandomLattices * sizeof(double*));
	for (unsigned int i = 0; i < kRandomLattices; i++) {
		nodeLocations[i] = malloc(columns * sizeof(double));
	}

	// initialise lattice agent tracking information
	double** agentLocationAllRealisations = malloc(kRandomLattices * sizeof(double*));
	for (unsigned int i = 0; i < kRandomLattices; i++) {
		agentLocationAllRealisations[i] = malloc((timeSteps + 1) * sizeof(double));
	}

	// for each max perturb distance investigated
	for (unsigned int i = 0; i < kNumBounds; i++) {

		// generation random lattices
		for (int latticeNum = 0; latticeNum < kRandomLattices; latticeNum++) {
			
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
					nodeLocations[latticeNum][j] = (double)xPosition;
				}
			}

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

			// store tracked agent information
			for (int i = 0; i < timeSteps + 1; i++) {
				agentLocationAllRealisations[latticeNum][i] = 
					(double)trackedPositions[i][0].column;
			}

			// clear lattice and deallocate memory
			lattice_clear(lattice, rows, columns, true);
			for (int i = 0; i < timeSteps + 1; i++) {
				free(trackedPositions[i]);
			}
			free(trackedPositions);
		}

		// save node locations
		sprintf(nodeLocationsFilename, "node_locations_%0.02f.txt", bounds[i]);
		bool latticeTracked = matrix_parser(nodeLocations, kRandomLattices, 
			columns, nodeLocationsFilename, "output/");
		if (latticeTracked) {
			printf("Stored node location information (delta: %0.02f).\n", bounds[i]);
		} else {
			printf("Error: failed storing node location information (delta: %0.02f).\n", 
				bounds[i]);
		}

		// save tracking information
		sprintf(agentLocationsFilename, "tracked_agent_positions_%0.02f.txt", bounds[i]);
		bool agentsTracked =  matrix_parser(agentLocationAllRealisations, kRandomLattices,
			timeSteps + 1, agentLocationsFilename, "output/");
		if (agentsTracked) {
			printf("Stored tracked agents information (delta: %0.02f).\n", bounds[i]);
		} else {
			printf("Error: failed storing tracked agents information (delta: %0.02f).\n", 
				bounds[i]);
		}
	}

	// deallocate memory
	lattice_destroy(&lattice, rows, columns, true);

	return EXIT_SUCCESS;
}
