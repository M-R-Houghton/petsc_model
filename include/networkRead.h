#ifndef NETWORK_READ_H
#define NETWORK_READ_H

#include "vectorOperations.h"

/**
 * \brief Initiates network read in routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkRead();

/**
 * \brief Reads box information from file.
 * \param file_ptr Pointer to the file.
 * \param box_ptr Empty box for data to be written into.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readBoxLine(char *line_ptr, Box **box_ptr);

#endif