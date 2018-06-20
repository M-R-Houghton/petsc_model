#ifndef NETWORK_WRITE_H
#define NETWORK_WRITE_H

#include "networkAnalysis.h"

/**
 * \brief Initiates network write out routine.
 * \param fileToWrite_ptr Name of file to write out to.
 * \param box_ptr Box containing network data to write out.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkWrite(const char *fileToWrite_ptr, Box *box_ptr);

/**
 * \brief Writes box information to file.
 * \param file_ptr Pointer to the file.
 * \param box_ptr Box containing network data to write out.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode writeBoxLine(FILE *file_ptr, Box *box_ptr);

/**
 * \brief Writes fibre information to file.
 * \param file_ptr Pointer to the file.
 * \param fibre_ptr Fibre containing network data to write out.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode writeFibreLine(FILE *file_ptr, Fibre *fibre_ptr);

/**
 * \brief Writes node information to file.
 * \param file_ptr Pointer to the file.
 * \param node_ptr Node containing network data to write out.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode writeNodeLine(FILE *file_ptr, Node *node_ptr);

#endif