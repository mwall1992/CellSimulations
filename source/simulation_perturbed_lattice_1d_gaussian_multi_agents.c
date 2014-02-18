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
	unsigned int columns = 300;
	lattice_t* lattice = lattice_create(rows, columns, 
		periodic, periodic, periodic, periodic);

	// initialise lattice positioning
	unsigned int const kNumStdDevs = 5;
	unsigned int const kStdDevsRepeatCount = 1;
	unsigned int const kRepeatCount = 1000;
	double stddevs[] = { 0.1, 0.2, 0.3, 0.4, 0.5 };

	unsigned int const kTimeSetsNum = 3;
	unsigned int timeSets[] = { 200, 500, 1000 };

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
	char latticeProfileFilename[100];

	// initialise agent tracking information
	unsigned int numTrackedAgents = 0;
	coordinate_t* trackedPositions = NULL;
	int* trackedAgentIds = NULL;

	// set motility properties
	double motilityProbability = 1.0;
	double xShiftPreference = 0;
	double yShiftPreference = 0;
	bool agentExclusion = false;

	// generation random lattices
	for (int stdDevIndex = 0; stdDevIndex < kNumStdDevs; stdDevIndex++) {
		
		for (int boundRepeatCount = 0; 
			boundRepeatCount < kStdDevsRepeatCount; boundRepeatCount++) {

			// generate any required random numbers (uniform dist)
			vslNewStream(&stream, BRNG, arc4random());	
			vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream, 
				columns, randomNumbers, 0.0f, stddevs[stdDevIndex]);

			// perturb and sort node locations
			for (int col = 0; col < columns; col++) {
				randomNumbers[col] += col;
			}
			qsort(randomNumbers, columns, sizeof(float), compare);

			// specify node locations
			for (int row = 0; row < rows; row++) {
				for (int col = 0; col < columns; col++) {
					coord.row = row;
					coord.column = col;
					xPosition = (double)randomNumbers[col];
					lattice_specify_position(lattice, coord, xPosition, yPosition);
				}
			}

			// save node locations
			bool saveNodeLocations = true;
			if (saveNodeLocations) {
				sprintf(latticeLayoutFileName, "node_positions_%0.02f_%d_ghosts.txt", 
					stddevs[stdDevIndex], boundRepeatCount);
				trackedLatticeLayout = lattice_parser_node_positions(lattice, rows, 
					columns, latticeLayoutFileName, "output/");
				if (!trackedLatticeLayout) {
					printf("Error: failed storing lattice layout information (case: %0.02f %d).\n", 
						stddevs[stdDevIndex], boundRepeatCount);
				}
			}

			// perform simulations
			bool performSimulation = true;
			if (performSimulation) {

				// perform simulations
				for (int repeatCount = 0; repeatCount < kRepeatCount; repeatCount++) {

					// populate lattice
					int* agentId;
					int currentAgentId = 1;
					coordinate_t agentPos;

					for (int j = 130; j < 171; j++) {
						agentId = malloc(sizeof(int));
						*agentId = currentAgentId++;

						agentPos.row = 0;
						agentPos.column = j;

						lattice_push_agent(lattice, agentPos, agentId);
					}

					// perform simulation
					for (int timeStep = 0; timeStep < timeSets[kTimeSetsNum-1]; 
							timeStep++) {
						performMotilityEvents(lattice, rows, columns, motilityProbability, 
							xShiftPreference, yShiftPreference, agentExclusion, 
							trackedAgentIds, numTrackedAgents, trackedPositions);

						for (int j = 0; j < kTimeSetsNum; j++) {
							if (timeStep == timeSets[j]-1) {
								// store lattice profile
								sprintf(latticeProfileFilename, "lattice_profile_%0.02f_%d_%d_%d_ghosts.txt", 
									stddevs[stdDevIndex], boundRepeatCount, repeatCount, timeStep+1);
								bool isTracked = lattice_occupancy_parser(lattice, rows, columns, 
									latticeProfileFilename, "output/");
								if (!isTracked) {
									printf("Error: failed to store lattice profile.\n");
								}
							}
						}
					}

					// clear lattice and deallocate memory
					lattice_clear(lattice, rows, columns, true);
				}
			}
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
