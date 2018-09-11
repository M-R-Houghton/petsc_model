#ifndef NETWORK_READ_H
#define NETWORK_READ_H

#include "vectorOperations.h"

/**
 * \brief Initiates network read in routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkRead();

/**
 * \brief Reads box information from a given line pointer
 * \param line_ptr Pointer to the line.
 * \param box_ptr Empty box for data to be written into.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readBoxLine(char *line_ptr, Box **box_ptr);

/**
 * \brief Reads node information from a given line pointer
 * \param line_ptr Pointer to the line.
 * \param box_ptr Empty box for data to be written into.
 * \param gIndex_ptr Pointer to the current global index.
 * \param gamma Node's gamma value.
 * \return gIndex to represent Petsc error code.
 */
PetscErrorCode readNodeLine(char *line_ptr, Box *box_ptr, PetscInt gIndex_ptr, PetscScalar gamma);

#endif