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
 * \brief Reads network input data from a given file name.
 * Depending on readCouplesOnly, it retrieves either box, fibre and node data and counts the 
 * number of couples (if any) or it retrieves the couple data.
 * \param fileToRead_ptr Pointer to the file to be read.
 * \param box_ptr Pointer to box for data to be written to.
 * \param readCouplesOnly Determines whether to read the couple data only or not.
 * \param cIndex_ptr Pointer to the current couple index.
 * \param gamma Global gamma value.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readInputFile(const char *fileToRead_ptr, Box **box_ptr_ptr, 
                                PetscBool readCouplesOnly, PetscInt *cIndex_ptr, PetscScalar gamma);
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
 * \param coupleCount Integer representing total number of unique couples.
 * \return Index to represent Petsc error code.
 */
PetscInt setInternalNodeIndices(Box *box_ptr, const PetscInt coupleCount);

/**
 * \brief Sets the global indices of all the standard internal nodes of a network
 * This can be used for indexing an uncoupled newtork, or for indexing any remaining 
 * internal nodes not associated with couples AFTER couple internal nodes have been 
 * given a shared index.
 * \param box_ptr Pointer to box containing the internal nodes.
 * \param nextIndex Pointer of the index to be assigned to all found internal nodes
 * \return Index to represent Petsc error code.
 */
PetscInt setStandardInternalNodeIndices(Box *box_ptr, PetscInt *nextIndex);

/**
 * \brief Sets the global indices of all the internal nodes of a coupled network
 * \param box_ptr Pointer to box containing the internal nodes.
 * \param coupleCount An integer counter for the number of couples in the network.
 * \param nextIndex Pointer to the next index to be assigned.
 * \return Index to represent Petsc error code.
 */
PetscInt setCoupledInternalNodeIndices(Box *box_ptr, const PetscInt coupleCount, PetscInt *nextIndex);

/**
 * \brief Checks global indices of all internal nodes have been set 
 * \param box_ptr Pointer to box containing the internal nodes.
 * \return Index to represent Petsc error code.
 */
void checkInternalNodeIndices(const Box *box_ptr);

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
 * \param gamma Node's gamma value.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readNodeLine(char *line_ptr, Box *box_ptr, PetscScalar gamma);

/**
 * \brief Reads couple information from a given line pointer
 * \param line_ptr Pointer to the line.
 * \param box_ptr Pointer to box for data to be written to.
 * \param coupleID Integer representing the global couple ID.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readCoupleLine(char *line_ptr, Box *box_ptr, const PetscInt coupleID);

/* 
 * \brief Tokenises a line into floats by splitting at whitespace
 * This function should not be passed any lines still containing non-numeric information
 * \param line_ptr Pointer to the line.
 * \param infoArray Array for storing info as floats from the split string
 * \return Integer representing number of elements in the array.
 */
PetscInt tokeniseLine(char *line_ptr, PetscScalar *infoArray);

/**
 * \brief Removes trailing whitespace on the right-hand side of a string
 * \param str_ptr
 * \return
 */
char *trimRightWhitespace(char *str_ptr);

#endif
