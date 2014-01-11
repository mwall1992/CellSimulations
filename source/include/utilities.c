#include "utilities.h"
#include <stdio.h>
#include <string.h>

#define CSV_DELIMITER ", "

bool matrix_parser(double** matrix, unsigned int rows, unsigned int columns, 
	char* filename, char* directory) {

	size_t absoluteFilenameLength = strlen(filename) + strlen(directory);
	char absoluteFilename[absoluteFilenameLength];

	strcpy(absoluteFilename, directory);
	strcat(absoluteFilename, filename);

	FILE* file = fopen(absoluteFilename, "w+");
	if (file != NULL) {

		int writeResult;
		
		for (unsigned int i = 0; i < rows; i++) {
			
			for (unsigned int j = 0; j < columns; j++) {
				writeResult = fprintf(file, "%f%s", matrix[i][j], 
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

bool vector_parser(double* vector, unsigned int rows, char* filename, 
	char* directory) {

	size_t absoluteFilenameLength = strlen(filename) + strlen(directory);
	char absoluteFilename[absoluteFilenameLength];

	strcpy(absoluteFilename, directory);
	strcat(absoluteFilename, filename);

	FILE* file = fopen(absoluteFilename, "w+");
	if (file != NULL) {

		int writeResult;
		
		for (unsigned int i = 0; i < rows; i++) {

			writeResult = fprintf(file, "%f%s\n", vector[i], CSV_DELIMITER);
			if (writeResult < 0) {
				fclose(file);
				return false;
			}
		}
	}

	int result = fclose(file);
	return (result != EOF);
}