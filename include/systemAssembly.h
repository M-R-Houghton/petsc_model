#ifndef SYSTEM_ASSEMBLY_H
#define SYSTEM_ASSEMBLY_H

#include "localAssembly.h"

/**
 * \brief Initiates system assembly routine.
 * \param H Global matrix to be assembled
 * \return Index to represent Petsc error code.
 */
PetscErrorCode systemAssembly(Mat H, Vec b);

/**
 * \brief Reads in a file of integers to an array
 * \param fileName String representing name of file to be read in.
 * \param array Array of integers to be written.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readInt(char *fileName, PetscInt *array, PetscInt n);

/**
 * \brief Reads in a file of doubles to an array
 * \param fileName String representing name of file to be read in.
 * \param array Array of doubles to be written.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readDbl(char *fileName, PetscScalar *array, PetscInt n);

#endif