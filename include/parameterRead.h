#ifndef PARAMETER_READ_H
#define PARAMETER_READ_H

#include "vectorOperations.h"

/**
 * \brief Checks for illegal filename lengths
 * \param inputFileName Pointer to the input file name to be checked.
 * \param outputFileName Pointer to the output file name to be checked.
 * \param resultsFileName Pointer to the results file name to be checked.
 */
void checkFileNameLengths(const char *inputFileName, const char *outputFileName,
                            const char *outadvFileName, const char *resultsFileName);

/**
 * \brief Initiates parameter file read in routine
 * \param fileToRead_ptr Pointer to the file to be read.
 * \param par_ptr Pointer to parameter struct to be built.
 * \param gamma Value of gamma to be assigned.
 * \param youngsModulus Value of Youngs Modulus to be assigned.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode parameterRead(const char *fileToRead_ptr, Parameters **par_ptr,
                                const PetscScalar gamma, const PetscScalar youngsModulus);

#endif
