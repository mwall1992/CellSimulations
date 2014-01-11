#include "lattice_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CSV_DELIMITER ", "

bool lattice_profile_parser(lattice_t* lattice, unsigned int rows, 
	unsigned int columns, char* filename, char* directory) {

	size_t absoluteFilenameLength = strlen(filename) + strlen(directory);
	char absoluteFilename[absoluteFilenameLength];

	strcpy(absoluteFilename, directory);
	strcat(absoluteFilename, filename);

	FILE* file = fopen(absoluteFilename, "w+");
	if (file != NULL) {

		coordinate_t coord;
		int writeResult;
		
		for (unsigned int i = 0; i < rows; i++) {
			
			for (unsigned int j = 0; j < columns; j++) {
				coord.row = i;
				coord.column = j;
				writeResult = fprintf(file, "%d%s", 
					(bool)lattice_get_agent_count(lattice, coord), 
					CSV_DELIMITER);

				if (writeResult < 0) {
					fclose(file);
					return false;
				}
			}

			writeResult = fprintf(file, "\n");
			if (writeResult < 0) {
				fclose(file);
				return false;
			}
		}
	}

	int result = fclose(file);
	return (result != EOF);
}

bool lattice_parser_node_positions(lattice_t* lattice, unsigned int rows,
	unsigned int columns, char* filename, char* directory) {

	size_t absoluteFilenameLength = strlen(filename) + strlen(directory);
	char absoluteFilename[absoluteFilenameLength];

	strcpy(absoluteFilename, directory);
	strcat(absoluteFilename, filename);

	FILE* file = fopen(absoluteFilename, "w+");
	if (file != NULL) {

		coordinate_t coord;
		int writeResult;

		double* xPosition = malloc(sizeof(double));
		double* yPosition = malloc(sizeof(double));
		
		for (unsigned int i = 0; i < rows; i++) {
			
			for (unsigned int j = 0; j < columns; j++) {
				coord.row = i;
				coord.column = j;
				lattice_get_specified_position(lattice, coord, xPosition, 
					yPosition);
				writeResult = fprintf(file, "%f%s%f%s\n", 
					*xPosition, CSV_DELIMITER,
					*yPosition, CSV_DELIMITER);

				if (writeResult < 0) {
					fclose(file);
					return false;
				}
			}
		}

		free(xPosition);
		free(yPosition);
	}

	int result = fclose(file);
	return (result != EOF);
}

bool tracked_agents_parser(coordinate_t** trackedAgentPositions, 
	unsigned int timeSteps, unsigned int trackedAgentIndex,
	 char* filename, char* directory) {

	size_t absoluteFilenameLength = strlen(filename) + strlen(directory);
	char absoluteFilename[absoluteFilenameLength];

	strcpy(absoluteFilename, directory);
	strcat(absoluteFilename, filename);

	FILE* file = fopen(absoluteFilename, "w+");
	if (file != NULL) {

		coordinate_t coord;
		int writeResult;

		for (unsigned int i = 0; i < timeSteps; i++) {
			coord = trackedAgentPositions[i][trackedAgentIndex];
			writeResult = fprintf(file, "%d%s%d%s\n",
				coord.row, CSV_DELIMITER, coord.column, CSV_DELIMITER);

			if (writeResult < 0) {
				fclose(file);
				return false;
			}
		}
	}

	int result = fclose(file);
	return (result != EOF);
}
