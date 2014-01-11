#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "include/simulation.h"
#include "include/lattice.h"
#include "include/lattice_parser.h"
#include "include/utilities.h"

int main(int argc, char* argv[]) {

	// construct lattice
	unsigned int rows = 20;
	unsigned int columns = 300;
	lattice_t* lattice = lattice_create(rows, columns, 
		periodic, periodic, periodic, periodic);

	// set motility properties
	double motilityProbability = 1.0;
	double xShiftPreference = 0;
	double yShiftPreference = 0;
	bool agentExclusion = true;

	// set agent tracking information
	unsigned int numTrackedAgents = 0;
	int* trackedAgentIds = NULL;
	coordinate_t* trackedPositions = NULL;

	// initialise simulation
	coordinate_t coord;
	int* agentId;
	int currentAgentCount = 0;

	int const realisationCount = 10;
	int const numStepSets = 3;
	int timeSteps[] = { 200, 500, 1000 };

	// initialise density information
	double* initialDensity;
	bool initialDensityFound = false;

	double* tempColumnDensities;
	double** averagedColumnDensities = malloc(numStepSets * sizeof(double*));
	
	for (int i = 0; i < numStepSets; i++) {
		averagedColumnDensities[i] = malloc(columns * sizeof(double));

		for (int j = 0; j < columns; j++) {
			averagedColumnDensities[i][j] = 0;
		}
	}

	// perform simulations
	int timeStep;
	for (int realisation = 0; realisation < realisationCount; realisation++) {
		
		// populate lattice
		currentAgentCount = 0;
		lattice_clear(lattice, rows, columns, true);
		for (int j = 129; j < 170; j++) {
			for (int i = 5; i < 15; i++) {
				coord.row = i;
				coord.column = j;

				// N.B. Be sure to free agent memory in lattice
				agentId = malloc(sizeof(int));
				*agentId = currentAgentCount++;

				lattice_push_agent(lattice, coord, agentId);
			}
		}

		// retrieve initial column densities
		if (!initialDensityFound) {
			initialDensity = determineColumnAgentDensity(lattice, rows, columns);
			initialDensityFound = true;
		}

		// perform a single simulation
		timeStep = 0;
		for (int stepSet = 0; stepSet < numStepSets; stepSet++) {
			for (; timeStep < timeSteps[stepSet]; timeStep++) {
				performMotilityEvents(lattice, rows, columns, motilityProbability, 
					xShiftPreference, yShiftPreference, agentExclusion, 
					trackedAgentIds, numTrackedAgents, trackedPositions);
			}

			// store density information
			tempColumnDensities = 
				determineColumnAgentDensity(lattice, rows, columns);
			for (int i = 0; i < columns; i++) {
				averagedColumnDensities[stepSet][i] = 
					averagedColumnDensities[stepSet][i] + tempColumnDensities[i];
			}
			free(tempColumnDensities);
		}
	}

	// calculate average column densities
	for (int i = 0; i < numStepSets; i++) {
		for (int j = 0; j < columns; j++) {
			averagedColumnDensities[i][j] = 
				averagedColumnDensities[i][j] / realisationCount;
		}
	}

	bool trackedColumnDensities;

	trackedColumnDensities = vector_parser(initialDensity, columns, 
		"densities_initial.txt", "output/");
	if (trackedColumnDensities) {
		printf("Stored agent column densities (initial set).\n");
	} else {
		printf("Error: failed to store agent column densities (initial set).\n");
	}

	trackedColumnDensities = vector_parser(averagedColumnDensities[0], columns, 
		"densities_1.txt", "output/");
	if (trackedColumnDensities) {
		printf("Stored agent column densities (first set).\n");
	} else {
		printf("Error: failed to store agent column densities (first set).\n");
	}

	trackedColumnDensities = vector_parser(averagedColumnDensities[1], columns, 
		"densities_2.txt", "output/");
	if (trackedColumnDensities) {
		printf("Stored agent column densities (second set).\n");
	} else {
		printf("Error: failed to store agent column densities (second set).\n");
	}

	trackedColumnDensities = vector_parser(averagedColumnDensities[2], columns, 
		"densities_3.txt", "output/");
	if (trackedColumnDensities) {
		printf("Stored agent column densities (third set).\n");
	} else {
		printf("Error: failed to store agent column densities (third set).\n");
	}

	// deallocate memory
	lattice_destroy(&lattice, rows, columns, true);
	free(initialDensity);

	for (int i = 0; i < numStepSets; i++) {
		free(averagedColumnDensities[i]);
	}
	free(averagedColumnDensities);

	return EXIT_SUCCESS;
}