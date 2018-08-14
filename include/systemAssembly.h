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
 * \brief Assembles tripod matrix.
 * \param rowFile String representing name of row pointer file to be read in.
 * \param colFile String representing name of column index file to be read in.
 * \param matFile String representing name of matrix value file to be read in.
 * \param vecFile String representing name of RHS vector file to be read in.
 * \param solFile String representing name of solution vector file to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode tripodAssembly(char const *rowFile, char const *colFile, char const *matFile, char const *vecFile, char const *solFile);

/**
 * \brief Reads in a file of integers to an array
 * \param fileName String representing name of file to be read in.
 * \param array Array of integers to be written.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readInt(char const *fileName, PetscInt *array, PetscInt n);

/**
 * \brief Reads in a file of doubles to an array
 * \param fileName String representing name of file to be read in.
 * \param array Array of doubles to be written.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readDbl(char const *fileName, PetscScalar *array, PetscInt n);

/**
 * \brief Writes out an array of doubles to file
 * \param fileName String representing name of file to be written to.
 * \param array Array of doubles to read from.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode writeDbl(char const *fileName, PetscScalar *array, PetscInt n) ;

#endif