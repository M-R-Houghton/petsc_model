#ifndef NETWORK_READ_H
#define NETWORK_READ_H

#include "parameterRead.h"

/**
 * \brief Initiates network read in routine.
 * \param fileToRead_ptr Pointer to the file to be read.
 * \param box_ptr Pointer to box for data to be written to.
 * \param gamma Global gamma value.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkRead(const char *fileToRead_ptr, Box **box_ptr_ptr, PetscScalar gamma);

/**
 * \brief Reads network data from a given line pointer
 * \param line_ptr Pointer to the line.
 * \param box_ptr Pointer to box for data to be written to.
 * \param cIndex_ptr Pointer to the current couple index.
 * \param gamma Global gamma value.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readDataLine(char *line_ptr, Box **box_ptr_ptr, PetscInt *cIndex_ptr, PetscScalar gamma);

/**
 * \brief Reads couple data from a given network
 * \param line_ptr Pointer to the file to be read.
 * \param box_ptr Pointer to box for data to be written to.
 * \param cCount Number of couples
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readCoupleData(char *line_ptr, Box *box_ptr, PetscInt *cCount);

/**
 * \brief Sets the global indices of all the internal nodes of a network
 * \param box_ptr Pointer to box containing the internal nodes.
 * \param coupledSystem Petsc boolean indicating whether the network has coupled nodes.
 * \return Index to represent Petsc error code.
 */
PetscInt setInternalNodeIndices(Box *box_ptr, PetscBool const coupledSystem, PetscInt coupleCount);

/**
 * \brief Sets the global indices of all the internal nodes of a standard network
 * \param box_ptr Pointer to box containing the internal nodes.
 * \return Index to represent Petsc error code.
 */
PetscInt setStandardInternalNodeIndices(Box *box_ptr);

/**
 * \brief Sets the global indices of all the internal nodes of a coupled network
 * \param box_ptr Pointer to box containing the internal nodes.
 * \param coupleCount An integer counter for the number of couples in the network.
 * \return Index to represent Petsc error code.
 */
PetscInt setCoupledInternalNodesIndices(Box *box_ptr, PetscInt const coupleCount);

/**
 * \brief Checks global indices of all internal nodes have been set 
 * \param box_ptr Pointer to box containing the internal nodes.
 * \return Index to represent Petsc error code.
 */
void checkInternalNodeIndices(Box const *box_ptr);

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
 * \brief Reads couple information from a given line pointer
 * \param line_ptr Pointer to the line.
 * \param box_ptr Pointer to box for data to be written to.
 * \param coupleID Integer representing the global couple ID.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readCoupleLine(char *line_ptr, Box *box_ptr, PetscInt const coupleID);

/**
 * \brief Removes trailing whitespace on the right-hand side of a string
 * \param str_ptr
 * \return
 */
char *trimRightWhitespace(char *str_ptr);

#endif
