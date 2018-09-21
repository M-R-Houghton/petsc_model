#ifndef PARAMETER_READ_H
#define PARAMETER_READ_H

#include "vectorOperations.h"

/**
 * \brief Initiates parameter file read in routine
 * \param fileToRead_ptr Pointer to the file to be read.
 * \param par_ptr Pointer to parameter struct to be built.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode parameterRead(const char *fileToRead_ptr, Parameters **par_ptr);

#endif