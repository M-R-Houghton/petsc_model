#ifndef NETWORK_READ_H
#define NETWORK_READ_H

#include "parameterRead.h"

/**
 * \brief Initiates network read in routine.
 * \param line_ptr Pointer to the file to be read.
 * \param box_ptr Pointer to box for data to be written to.
 * \param gamma Global gamma value.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkRead(const char *fileToRead_ptr, Box **box_ptr_ptr, PetscScalar gamma);

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
 * \brief Sets the global indices of all the internal nodes of a network
 * \param box_ptr Pointer to box containing the internal nodes.
 * \param coupledSystem Petsc boolean indicating whether the network has coupled nodes.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode setInternalNodeIndices(Box *box_ptr, PetscBool coupledSystem);

/**
 * \brief Sets the global indices of all the internal nodes of a standard network
 * \param box_ptr Pointer to box containing the internal nodes.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode setStandardInternalNodeIndices(Box *box_ptr);

/**
 * \brief Sets the global indices of all the internal nodes of a coupled network
 * \param box_ptr Pointer to box containing the internal nodes.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode setCoupledInternalNodeIndices(Box *box_ptr);

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
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readNodeLine(char *line_ptr, Box *box_ptr, PetscInt *gIndex_ptr, PetscScalar gamma);

/**
 * \brief Reads coupling information from a given line pointer
 * \param line_ptr Pointer to the line.
 * \param box_ptr Pointer to box for data to be written to.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readCouplingLine(char *line_ptr, Box *box_ptr);

/**
 * \brief Removes trailing whitespace on the right-hand side of a string
 * \param str_ptr
 * \return
 */
char *trimRightWhitespace(char *str_ptr);

#endif
