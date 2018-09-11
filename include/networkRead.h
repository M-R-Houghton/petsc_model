#ifndef NETWORK_READ_H
#define NETWORK_READ_H

#include "vectorOperations.h"

/**
 * \brief Initiates network read in routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkRead();

/**
 * \brief Reads network data from a given line pointer
 * \param line_ptr Pointer to the line.
 * \param box_ptr Pointer to box for data to be written to.
 * \param gIndex_ptr Pointer to the current global index.
 * \param gamma Global gamma value.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readDataLine(char *line_ptr, Box **box_ptr_ptr, PetscInt *gIndex_ptr, PetscScalar gamma);

/**
 * \brief Reads box information from a given line pointer
 * \param line_ptr Pointer to the line.
 * \param box_ptr Empty box for data to be written into.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readBoxLine(char *line_ptr, Box **box_ptr);

/**
 * \brief Reads fibre information from a given line pointer
 * \param line_ptr Pointer to the line.
 * \param box_ptr Pointer to box for data to be written to.
 */
PetscErrorCode readFibreLine(char *line_ptr, Box *box_ptr);

/**
 * \brief Reads node information from a given line pointer
 * \param line_ptr Pointer to the line.
 * \param box_ptr Pointer to box for data to be written to.
 * \param gIndex_ptr Pointer to the current global index.
 * \param gamma Node's gamma value.
 * \return gIndex to represent Petsc error code.
 */
PetscErrorCode readNodeLine(char *line_ptr, Box *box_ptr, PetscInt *gIndex_ptr, PetscScalar gamma);

/**
 * \brief Removes trailing whitespace on the right-hand side of a string
 * \param str_ptr
 * \return
 */
char *trimRightWhitespace(char *str_ptr);

#endif