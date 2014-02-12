#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "include/simulation.h"
#include "include/lattice.h"
#include "include/lattice_parser.h"

int main(int argc, char* argv[]) {

	// construct lattice
	unsigned int rows = 5;
	unsigned int columns = 5;
	lattice_t* lattice = lattice_create(rows, columns, 
		periodic, periodic, periodic, periodic);

	// populate lattice
	int* agentId = malloc(sizeof(int));
	*agentId = 1;

	coordinate_t agentPos = { .row = 2, .column = 2 };
	lattice_push_agent(lattice, agentPos, agentId);	

	// set motility properties
	double proliferationProbability = 1.0;
	proliferation_type_t proliferationType = ballistic;
	unsigned int proliferationDelta = 4;
	bool agentExclusion = true;

	// initialise agent tracking information
	unsigned int numTrackedAgents = 1;
	unsigned int timeSteps = 1;
	
	coordinate_t** trackedPositions = malloc((timeSteps + 1) * sizeof(coordinate_t*));
	for (int i = 0; i < timeSteps + 1; i++) {
		trackedPositions[i] = malloc(numTrackedAgents * sizeof(coordinate_t));
	}

	int* trackedAgentIds = malloc(numTrackedAgents * sizeof(int));
	trackedAgentIds[0] = 1;
	trackedPositions[0][0] = agentPos;

	// Perform simulation
	for (int i = 1; i < timeSteps + 1; i++) {
		performProliferationEvents(lattice, rows, columns, 
			proliferationProbability, agentExclusion, proliferationType, 
			proliferationDelta, trackedAgentIds, numTrackedAgents, 
			trackedPositions[i]);
	}

	// Save proliferation information
	bool isTracked = lattice_profile_parser(lattice, rows, columns, 
		"prolif_mat.txt", "output/");
	if (isTracked) {
		printf("Stored lattice profile.\n");
	} else {
		printf("Error: failed to store lattice profile.\n");
	}

	bool agentIsTracked = tracked_agents_parser(trackedPositions, timeSteps + 1,
		0, "tracked_agent.txt", "output/");
	if (agentIsTracked) {
		printf("Stored agent tracking information.\n");
	} else {
		printf("Error: failed to store agent tracking information.\n");
	}

	// deallocate memory
	lattice_destroy(&lattice, rows, columns, true);
	free(trackedAgentIds);

	for (int i = 0; i < timeSteps; i++) {
		free(trackedPositions[i]);
	}
	free(trackedPositions);

	return EXIT_SUCCESS;
}
