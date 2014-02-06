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
	unsigned int const kNumBounds = 1;
	unsigned int const kBoundRepeatCount = 1;
	unsigned int const kRepeatCount = 2000;
	double bounds[] = { 0.1 };

	// set temporal properties
	unsigned int timeSteps = 200;

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
	bool trackedLatticeProfile;
	char latticeProfileFilename[50];

	// initialise agent tracking information
	unsigned int numTrackedAgents = 0;
	int* trackedAgentIds = NULL;
	coordinate_t* trackedPositions = NULL;

	// set motility properties
	double motilityProbability = 1.0;
	double xShiftPreference = 0;
	double yShiftPreference = 0;
	bool agentExclusion = true;

	// generation random lattices
	for (int i = 0; i < kNumBounds; i++) {
		
		for (int boundRepeatCount = 0; 
			boundRepeatCount < kBoundRepeatCount; boundRepeatCount++) {

			float randomSeed = 874595;

			// generate any required random numbers (uniform dist)
			vslNewStream(&stream, BRNG, randomSeed);	
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
				sprintf(latticeLayoutFileName, "node_positions_%0.02f_%d_%d.txt", 
					bounds[i], boundRepeatCount, repeatCount);
				trackedLatticeLayout = lattice_parser_node_positions(lattice, rows, 
					columns, latticeLayoutFileName, "output/");
				if (!trackedLatticeLayout) {
					printf("Error: failed storing lattice layout information (case: %0.02f %d).\n", 
						bounds[i], boundRepeatCount);
				}
			}

			// perform simulations
			bool performSimulation = true;
			if (performSimulation) {

				// perform simulations
				for (int repeatCount = 0; repeatCount < kRepeatCount; repeatCount++) {

					// populate lattice
					int* agentId;
					coordinate_t agentPos;

					int currentAgentCount = 1;

					for (int i = 0; i < rows; i++) {
						for (int j = 130; j < 170; j++) {
							agentPos.row = i;
							agentPos.column = j;
							agentId = malloc(sizeof(int));
							*agentId = currentAgentCount++;
							lattice_push_agent(lattice, agentPos, agentId);
						}
					}

					// perform simulation
					for (int i = 0; i < timeSteps; i++) {
						performMotilityEvents(lattice, rows, columns, motilityProbability, 
							xShiftPreference, yShiftPreference, agentExclusion, 
							trackedAgentIds, numTrackedAgents, trackedPositions);
					}

					// Save tracked information
					sprintf(latticeProfileFilename, "lattice_profile_%0.02f_%d_%d.txt", 
						bounds[i], boundRepeatCount, repeatCount);
					trackedLatticeProfile = lattice_profile_parser(lattice, rows, columns, 
						latticeProfileFilename, "output/");
					if (!trackedLatticeProfile) {
						printf("Error: failed to store lattice profile (case: %0.02f %d %d).\n", 
							bounds[i], boundRepeatCount, repeatCount);
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
