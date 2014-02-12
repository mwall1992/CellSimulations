#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "include/simulation.h"
#include "include/lattice.h"
#include "include/lattice_parser.h"

int main(int argc, char* argv[]) {

	// construct lattice
	unsigned int rows = 3;
	unsigned int columns = 3;
	lattice_t* lattice = lattice_create(rows, columns, 
		periodic, periodic, periodic, periodic);

	// populate lattice
	int* agentId = malloc(sizeof(int));
	*agentId = 1;

	coordinate_t agentPos = { .row = 0, .column = 0 };
	lattice_push_agent(lattice, agentPos, agentId);	

	int* blockingAgentId = malloc(sizeof(int));
	*blockingAgentId = 2;

	coordinate_t blockingAgentPos = { .row = 2, .column = 2 };
	lattice_push_agent(lattice, blockingAgentPos, blockingAgentId);

	// set motility properties
	double motilityProbability = 1.0;
	double xShiftPreference = 0;
	double yShiftPreference = 0;
	bool agentExclusion = true;

	// initialise agent tracking information
	unsigned int numTrackedAgents = 2;
	unsigned int timeSteps = 1;
	
	coordinate_t** trackedPositions = malloc((timeSteps + 1) * sizeof(coordinate_t*));
	for (int i = 0; i < timeSteps + 1; i++) {
		trackedPositions[i] = malloc(numTrackedAgents * sizeof(coordinate_t));
	}

	int* trackedAgentIds = malloc(numTrackedAgents * sizeof(int));
	trackedAgentIds[0] = 1;
	trackedPositions[0][0] = agentPos;
	trackedAgentIds[1] = 2;
	trackedPositions[0][1] = blockingAgentPos;

	// Perform simulation
	for (int i = 1; i < timeSteps + 1; i++) {
		performMotilityEvents(lattice, rows, columns, motilityProbability, 
			xShiftPreference, yShiftPreference, agentExclusion, 
			trackedAgentIds, numTrackedAgents, trackedPositions[i]);
	}

	// Save tracked information
	bool isTracked = lattice_profile_parser(lattice, rows, columns, 
		"motile_mat.txt", "output/");
	if (isTracked) {
		printf("Stored lattice profile.\n");
	} else {
		printf("Error: failed to store lattice profile.\n");
	}

	bool isTrackedFirstAgent = tracked_agents_parser(trackedPositions, 
		timeSteps + 1, 0, "tracked_1.txt", "output/");
	bool isTrackedSecondAgent =  tracked_agents_parser(trackedPositions, 
		timeSteps + 1, 1, "tracked_2.txt", "output/");
	if (isTrackedFirstAgent && isTrackedSecondAgent) {
		printf("Stored tracking information.\n");
	} else {
		printf("Write Error: failed storing tracking information.\n");
	}

	// deallocate memory
	lattice_destroy(&lattice, rows, columns, true);
	free(trackedAgentIds);

	for (int i = 0; i < timeSteps + 1; i++) {
		free(trackedPositions[i]);
	}
	free(trackedPositions);

	return EXIT_SUCCESS;
}
