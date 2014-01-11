#ifndef _utilities_h
#define _utilities_h

#include <stdbool.h>

bool matrix_parser(double** matrix, unsigned int rows, unsigned int columns, 
	char* filename, char* directory);

bool vector_parser(double* vector, unsigned int rows, char* filename, 
	char* directory);

#endif