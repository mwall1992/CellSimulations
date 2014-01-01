#include "lattice.h"
#include <stdbool.h>

#ifndef _simulation_h
#define _simulation_h

typedef enum proliferation_type proliferation_type_t;

enum proliferation_type {
	ballistic,
	adjacent
};

void performMotilityEvents(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, double motilityProbability, double xShiftPreference, 
	double yShiftPreference, bool agentExclusion, int* trackedAgentIds, 
	unsigned int numTrackedAgents, coordinate_t* trackedPositions);

void performProliferationEvents(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, double proliferationProbability, bool agentExclusion, 
	proliferation_type_t proliferationType, unsigned int proliferationDelta, 
	int* trackedAgentIds, unsigned int numTrackedAgents, 
	coordinate_t* trackedPositions);

coordinate_t determineAgentMoveLocation(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, double xShiftPreference, double yShiftPreference, 
	coordinate_t anchor, float directionProb);

void determineProliferationLocations(lattice_t* lattice, unsigned int rows,
	unsigned int columns, proliferation_type_t proliferationType, 
	unsigned int proliferationDelta, float directionProb, 
	coordinate_t* parentCoord, coordinate_t* daughterCoord);

#endif
