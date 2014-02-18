#include "simulation.h"
#include <stdlib.h>
#include <math.h>
#include <mkl.h>

#include <stdio.h>

#define BRNG VSL_BRNG_MCG31
#define RANDOM_NUMS_PER_EVENT 3

#define HORIZONTAL_ONLY 1

void performMotilityEvents(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, double motilityProbability, double xShiftPreference, 
	double yShiftPreference, bool agentExclusion, int* trackedAgentIds, 
	unsigned int numTrackedAgents, coordinate_t* trackedPositions) {

	// inspect the initial agent population
	unsigned int initialAgentCount = 
		lattice_get_total_agent_count(lattice, rows, columns);
	int randomNumbersCount = initialAgentCount * RANDOM_NUMS_PER_EVENT;

	// generate any required random numbers (uniform dist)
	float randomNumbers[randomNumbersCount];
	VSLStreamStatePtr stream;
	vslNewStream(&stream, BRNG, arc4random());
	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 
		randomNumbersCount, randomNumbers, 0.0f, 1.0f);

	// perform a series of motility events
	unsigned int agentNum;
	int* agentId;
	unsigned int* nodeAgentIndex = (unsigned int*)malloc(sizeof(unsigned int));
	unsigned int randomNumIndex = 0;
	coordinate_t agentPosition;
	coordinate_t agentProposedMove;

	// for the initial number of agents
	for (int i = 0; i < initialAgentCount; i++) {

		// retrieve agent information
		agentNum = (unsigned int)ceil(randomNumbers[randomNumIndex++] 
			* initialAgentCount);
		if (!agentNum) {
				agentNum++;
		}

		agentPosition = lattice_retrieve_agent_coord(lattice, rows, columns, 
			agentNum, nodeAgentIndex);
		agentId = lattice_get_agent(lattice, agentPosition, *nodeAgentIndex);

		// attempt to move agent
		if (randomNumbers[randomNumIndex++] < (float)motilityProbability) {
			agentProposedMove = determineAgentMoveLocation(lattice, rows, 
				columns, xShiftPreference, yShiftPreference, agentPosition, 
				randomNumbers[randomNumIndex++]);

			if (!agentExclusion 
				|| (agentExclusion && !lattice_get_agent_count(lattice, agentProposedMove))) {

				lattice_move_agent(lattice, agentPosition, *nodeAgentIndex, 
					agentProposedMove);
				agentPosition = agentProposedMove;
			}
		}
	}

	if (lattice_get_total_agent_count(lattice, rows, columns) != initialAgentCount) {
		printf("Error: lost agents.\n");
	}

	// attempt to track agent movement
	coordinate_t trackedCoord;
	for (int i = 0; i < numTrackedAgents; i++) {
		trackedCoord = lattice_find_agent(lattice, rows, columns, &trackedAgentIds[i]);

		if (trackedCoord.row <= rows && trackedCoord.column <= columns) {
			trackedPositions[i] = trackedCoord;
		} else {
			printf("Error: unable to find tracked agent coordinate (id: %d).\n",
				trackedAgentIds[i]);
		}
	}

	free(nodeAgentIndex);
}

void performProliferationEvents(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, double proliferationProbability, bool agentExclusion, 
	proliferation_type_t proliferationType, unsigned int proliferationDelta, 
	int* trackedAgentIds, unsigned int numTrackedAgents, 
	coordinate_t* trackedPositions) {

	// inspect the initial agent population
	unsigned int initialAgentCount = 
		lattice_get_total_agent_count(lattice, rows, columns);
	int randomNumbersCount = initialAgentCount * RANDOM_NUMS_PER_EVENT;

	// generate any required random numbers (uniform dist)
	float randomNumbers[randomNumbersCount];
	VSLStreamStatePtr stream;
	vslNewStream(&stream, BRNG, arc4random());
	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 
		randomNumbersCount, randomNumbers, 0.0f, 1.0f);

	// perform a series of proliferation events
	unsigned int currentAgentCount = initialAgentCount;
	unsigned int agentNum;
	int* agentId;
	unsigned int* nodeAgentIndex = (unsigned int*)malloc(sizeof(unsigned int));
	unsigned int randomNumIndex = 0;
	coordinate_t agentPosition;
	coordinate_t* parentCoord = malloc(sizeof(coordinate_t));
	coordinate_t* daughterCoord = malloc(sizeof(coordinate_t));

	// for the initial number of agents
	for (int i = 0; i < initialAgentCount; i++) {

		// retrieve agent information
		agentNum = (unsigned int)ceil(randomNumbers[randomNumIndex++] 
				* initialAgentCount);
		if (!agentNum) {
			agentNum++;
		}

		agentPosition = lattice_retrieve_agent_coord(lattice, rows, columns, 
			agentNum, nodeAgentIndex);
		agentId = lattice_get_agent(lattice, agentPosition, *nodeAgentIndex);

		// attempt to divide agent
		if (randomNumbers[randomNumIndex++] < (float)proliferationProbability) {

			*parentCoord = agentPosition;
			*daughterCoord = agentPosition;		

			determineProliferationLocations(lattice, rows, columns, 
				proliferationType, proliferationDelta, randomNumbers[randomNumIndex++], 
				parentCoord, daughterCoord);

			bool daughterCoordOccupied = 
				lattice_get_agent_count(lattice, *daughterCoord);

			bool parentSafe = true;
			if (!( ((*parentCoord).row == agentPosition.row) 
				&& ((*parentCoord).column == agentPosition.column) )) {
				parentSafe = !lattice_get_agent_count(lattice, *parentCoord);
			}

			if (!agentExclusion 
				|| (agentExclusion && !daughterCoordOccupied && parentSafe)) {

				lattice_move_agent(lattice, agentPosition, *nodeAgentIndex, 
					*parentCoord);
				agentPosition = *parentCoord;

				// N.B. Be sure to free agent id memory via lattice
				int* daughterId = malloc(sizeof(int));
				*daughterId = ++currentAgentCount;
				lattice_push_agent(lattice, *daughterCoord, daughterId);
			}
		}
	}

	// attempt to track agent movement
	coordinate_t trackedCoord;
	for (int i = 0; i < numTrackedAgents; i++) {
		trackedCoord = lattice_find_agent(lattice, rows, columns, &trackedAgentIds[i]);

		if (trackedCoord.row <= rows && trackedCoord.column <= columns) {
			trackedPositions[i] = trackedCoord;
		} else {
			printf("Error: unable to find tracked agent coordinate (id: %d).\n",
				trackedAgentIds[i]);
		}
	}

	free(parentCoord);
	free(daughterCoord);
}

coordinate_t determineAgentMoveLocation(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, double xShiftPreference, double yShiftPreference, 
	coordinate_t anchor, float directionProb) {

	coordinate_t moveCoord = anchor;

	int delta = 1;
	orientation_t deltaOrientation;

	float xPreferenceBound = (float)(1 - xShiftPreference) / 4.0;
	float yPreferenceBound = (float)(1 - yShiftPreference) / 4.0;

	if (HORIZONTAL_ONLY) {
		if (directionProb < 1.0/2.0) {
			delta *= -1;
			deltaOrientation = horizontal;
		} else {
			deltaOrientation = horizontal;
		}
	} else {
		if (directionProb < yPreferenceBound) {
			delta *= -1;
			deltaOrientation = vertical;
		} else if (directionProb < 1.0/2.0) {
			deltaOrientation = vertical;
		} else if (directionProb < 1.0/2.0 + xPreferenceBound) {
			delta *= -1;
			deltaOrientation = horizontal;
		} else {
			deltaOrientation = horizontal;
		}
	}

	return lattice_retrieved_adjacent_coord(lattice, rows, columns, anchor, delta, 
		deltaOrientation);
}

void determineProliferationLocations(lattice_t* lattice, unsigned int rows,
	unsigned int columns, proliferation_type_t proliferationType, 
	unsigned int proliferationDelta, float directionProb, 
	coordinate_t* parentCoord, coordinate_t* daughterCoord) {

	int parentDelta = proliferationDelta;
	int daughterDelta = proliferationDelta;
	orientation_t deltaOrientation;

	*daughterCoord = *parentCoord;

	switch (proliferationType) {
		case ballistic: {
			parentDelta = -1 * parentDelta / 2;
			daughterDelta = daughterDelta / 2;

			if (directionProb < 1.0/2.0) {
				deltaOrientation = vertical;
			} else {
				deltaOrientation = horizontal;
			}
		}
		break;

		case adjacent: {
			parentDelta = 0;
			
			if (directionProb < 1.0/4.0) {
				daughterDelta *= -1;
				deltaOrientation = vertical;
			} else if (directionProb < 1.0/2.0) {
				deltaOrientation = vertical;
			} else if (directionProb < 3.0/4.0) {
				daughterDelta *= -1;
				deltaOrientation = horizontal;
			} else {
				deltaOrientation = horizontal;
			}
		}
		break;
	}

	*parentCoord = lattice_retrieved_adjacent_coord(lattice, rows, columns, 
		*parentCoord, parentDelta, deltaOrientation);
	*daughterCoord = lattice_retrieved_adjacent_coord(lattice, rows, columns, 
		*daughterCoord, daughterDelta, deltaOrientation);
}

double* determineColumnAgentDensity(lattice_t* lattice, unsigned int rows,
	unsigned int columns) {

	double* agentColumnDensities = malloc(columns * sizeof(double));

	int agentOccupancyCount;
	coordinate_t coord;

	for (int i = 0; i < columns; i++) {

		agentOccupancyCount = 0;
		for (int j = 0; j < rows; j++) {
			coord.row = j;
			coord.column = i;

			if (lattice_get_agent_count(lattice, coord)) {
				agentOccupancyCount++;
			}
		}

		agentColumnDensities[i] = (double)agentOccupancyCount / (double)rows;
	}

	return agentColumnDensities;
}
