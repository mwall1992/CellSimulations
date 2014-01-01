#include "lattice_parser.h"
#include <stdio.h>
#include <string.h>

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
