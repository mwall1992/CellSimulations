##
# Filename: makefile
# author: Morgan Wall
# date: 27-12-2013
#
# The makefile used for compiling cell simulation programs using static linking 
# of the Intel MKL libraries for a sequential (i.e. single threaded) execution.
#
# Note: Linker and compiler options were chosen based on the Intel® Math Kernel 
# Library Link Line Advisor. See here:
# http://software.intel.com/en-us/articles/intel-mkl-link-line-advisor
#
# Implementation note: The Intel MKL libraries are required for compiling this 
# program. This makefile assumes that an environmental variable called "MKLROOT"
# exists and points to the root directory of the standard MKL installation.
##

# Compiler and Linker settings
CC = icc
COMPILER_FLAGS = -std=c99 -DMKL_ILP64 -I$(MKLROOT)/include
LIBRARIES =  $(MKLROOT)/lib/libmkl_intel_ilp64.a \
	$(MKLROOT)/lib/libmkl_core.a \
	$(MKLROOT)/lib/libmkl_sequential.a 
LINKER_FLAGS = -lpthread -lm

# Directories
EXE_DIR = executables
INPUT_DIR = input
OUTPUT_DIR = output
SOURCE_DIR = source
INCLUDE_DIR = $(SOURCE_DIR)/include
BACKUP_DIR = backup

# Files
DRIVER_FILE = $(SOURCE_DIR)/simulation_avg_over_lattices_perturbed.c
INCLUDE_FILES = $(shell find $(INCLUDE_DIR) -name *.c) \
	$(shell find $(INCLUDE_DIR) -name *.h)
FILES = $(DRIVER_FILE) $(INCLUDE_FILES)
OUT = $(EXE_DIR)/simulation

# Targets
.PHONY: clean output_clean backup

install: clean
	mkdir $(EXE_DIR)
	mkdir $(INPUT_DIR)
	mkdir $(OUTPUT_DIR)

build: install
	$(CC) $(FILES) -o $(OUT) $(COMPILER_FLAGS) $(LIBRARIES) $(LINKER_FLAGS)

rebuild: output_clean build

output_clean:
	rm -fr $(EXE_DIR)/*
	rm -fr $(OUTPUT_DIR)/*

clean: output_clean
	rm -fr $(INPUT_DIR)/*
	rm -fr $(EXE_DIR)
	rm -fr $(INPUT_DIR)
	rm -fr $(OUTPUT_DIR)

backup:
	mkdir $(BACKUP_DIR)
	cp -r $(EXE_DIR) $(BACKUP_DIR)/$(EXE_DIR)
	cp -r $(INPUT_DIR) $(BACKUP_DIR)/$(INPUT_DIR)
	cp -r $(OUTPUT_DIR) $(BACKUP_DIR)/$(OUTPUT_DIR)
	cp -r $(SOURCE_DIR) $(BACKUP_DIR)/$(SOURCE_DIR)
