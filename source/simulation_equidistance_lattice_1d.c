#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "include/simulation.h"
#include "include/lattice.h"
#include "include/lattice_parser.h"

int main(int argc, char* argv[]) {

	// construct lattice
	unsigned int rows = 1;
	unsigned int columns = 200;
	lattice_t* lattice = lattice_create(rows, columns, 
		periodic, periodic, periodic, periodic);

	// populate lattice
	int* agentId = malloc(sizeof(int));
	*agentId = 1;
	coordinate_t coord = { .row = 0, .column = columns/2 };
	lattice_push_agent(lattice, coord, agentId);

	// set motility properties
	double motilityProbability = 1.0;
	double xShiftPreference = 0;
	double yShiftPreference = 0;
	bool agentExclusion = false;

	// initialise agent tracking information
	unsigned int numTrackedAgents = 1;
	unsigned int timeSteps = 500;
	
	coordinate_t** trackedPositions = malloc((timeSteps + 1) * sizeof(coordinate_t*));
	for (int i = 0; i < timeSteps + 1; i++) {
		trackedPositions[i] = malloc(numTrackedAgents * sizeof(coordinate_t));
	}

	// specify tracked agent
	int* trackedAgentIds = malloc(numTrackedAgents * sizeof(int));
	trackedAgentIds[0] = 1;
	trackedPositions[0][0] = coord;

	// Perform simulation
	for (int i = 1; i < timeSteps + 1; i++) {
		performMotilityEvents(lattice, rows, columns, motilityProbability, 
			xShiftPreference, yShiftPreference, agentExclusion, 
			trackedAgentIds, numTrackedAgents, trackedPositions[i]);
	}

	// Save proliferation information
	bool agentIsTracked = tracked_agents_parser(trackedPositions, timeSteps + 1,
		0, "simulation_tracked_agent.txt", "output/");
	if (agentIsTracked) {
		printf("Stored agent tracking information.\n");
	} else {
		printf("Error: failed to store agent tracking information.\n");
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
