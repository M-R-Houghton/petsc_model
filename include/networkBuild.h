#ifndef NETWORK_BUILD_H
#define NETWORK_BUILD_H

#include "common.h"

/**
 * \brief Initiates network build routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkBuild();

/**
 * \brief Creates a parameters structure
 * \param gamma Parameter representing shear strain
 * \param yMod Parameter representing the Youngs Modulus value
 * \return
 */
Parameters *makeParameters(PetscScalar gamma, PetscScalar yMod);

/**
 * \brief Creates a sparse structure
 * \param n Total number of elements
 * \param nz Number of nonzero elements
 * \return
 */
Sparse *makeSparse(PetscInt n, PetscInt nz);

/**
 * \brief Checks box arguments are all legal
 * \param nCount
 * \param fCount
 * \param xDim
 * \param yDim
 * \param zDim
 * \param xPer
 * \param yPer
 * \param zPer
 * \return
 */
PetscInt checkBoxArguments(PetscInt nCount, PetscInt fCount, 
							PetscScalar xDim, PetscScalar yDim, PetscScalar zDim, 
							PetscInt xPer, PetscInt yPer, PetscInt zPer);

/**
 * \brief Creates a box structure
 * \param nCount
 * \param fCount
 * \param xDim
 * \param yDim
 * \param zDim
 * \param xPer
 * \param yPer
 * \param zPer
 * \return
 */
Box *makeBox(PetscInt nCount, PetscInt fCount, 
				PetscScalar xDim, PetscScalar yDim, PetscScalar zDim, 
				PetscInt xPer, PetscInt yPer, PetscInt zPer);

#endif