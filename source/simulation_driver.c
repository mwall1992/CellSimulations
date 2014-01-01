#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "include/simulation.h"
#include "include/lattice.h"
#include "include/lattice_parser.h"

int main(int argc, char* argv[]) {

	// construct lattice
	unsigned int rows = 5;
	unsigned int columns = 30;
	lattice_t* lattice = lattice_create(rows, columns, 
		periodic, periodic, periodic, periodic);

	// populate lattice
	coordinate_t coord;
	int* agentId;
	int currentAgentCount = 1;

	for (int j = 10; j < 20; j++) {
		for (int i = 0; i < rows; i++) {
			coord.row = i;
			coord.column = j;

			// memory leak
			agentId = malloc(sizeof(int));
			*agentId = currentAgentCount++;

			lattice_push_agent(lattice, coord, agentId);
		}
	}

	// set motility properties
	double motilityProbability = 1.0;
	double xShiftPreference = 0;
	double yShiftPreference = 0;
	bool agentExclusion = true;

	// set proliferation properties
	double proliferationProbability = 0.001;
	proliferation_type_t proliferationType = adjacent;
	unsigned int proliferationDelta = 1;

	// initialise agent tracking information
	unsigned int numTrackedAgents = 1;
	unsigned int timeSteps = 40;
	
	coordinate_t** trackedPositions = malloc((timeSteps + 1) * sizeof(coordinate_t*));
	for (int i = 0; i < timeSteps + 1; i++) {
		trackedPositions[i] = malloc(numTrackedAgents * sizeof(coordinate_t));
	}

	int* trackedAgentIds = malloc(numTrackedAgents * sizeof(int));
	trackedAgentIds[0] = currentAgentCount - 1;

	coordinate_t initialCoord = { .row = rows - 1, .column = 19 };
	trackedPositions[0][0] = initialCoord;

	unsigned int totalAgentCount = 
		lattice_get_total_agent_count(lattice, rows, columns);
	printf("Initial agent count: %d\n", totalAgentCount);

	// Perform simulation
	for (int i = 1; i < timeSteps + 1; i++) {
		performMotilityEvents(lattice, rows, columns, motilityProbability, 
			xShiftPreference, yShiftPreference, agentExclusion, 
			trackedAgentIds, numTrackedAgents, trackedPositions[i]);

		performProliferationEvents(lattice, rows, columns, 
			proliferationProbability, agentExclusion, proliferationType, 
			proliferationDelta, trackedAgentIds, numTrackedAgents, 
			trackedPositions[i]);
	}

	totalAgentCount = 
		lattice_get_total_agent_count(lattice, rows, columns);
	printf("Initial agent count: %d\n", totalAgentCount);

	// Save proliferation information
	bool agentIsTracked = tracked_agents_parser(trackedPositions, timeSteps + 1,
		0, "simulation_tracked_agent.txt", "output/");
	if (agentIsTracked) {
		printf("Stored agent tracking information.\n");
	} else {
		printf("Error: failed to store agent tracking information.\n");
	}

	bool isTracked = lattice_profile_parser(lattice, rows, columns, 
		"simulation_mat.txt", "output/");
	if (isTracked) {
		printf("Stored lattice profile.\n");
	} else {
		printf("Error: failed to store lattice profile.\n");
	}

	// deallocate memory
	lattice_destroy(&lattice, rows, columns);
	free(agentId);
	free(trackedAgentIds);

	for (int i = 0; i < timeSteps + 1; i++) {
		free(trackedPositions[i]);
	}
	free(trackedPositions);

	return EXIT_SUCCESS;
}
