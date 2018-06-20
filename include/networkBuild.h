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
 */
void checkBoxArguments(PetscInt nCount, PetscInt fCount, 
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

/**
 * \brief Checks fibre arguments are all legal
 * \param box_ptr
 * \param fID
 * \param nOnFibre
 * \param radius
 * \param nList_ptr_ptr
 */
void checkFibreArguments(Box *box_ptr, PetscInt fID, PetscInt nOnFibre, 
				PetscScalar radius, Node **nList_ptr_ptr);

/**
 * \brief Creates a fibre within its allocated location in a box
 * \param box_ptr
 * \param fID
 * \param nOnFibre
 * \param radius
 * \param nList_ptr_ptr
 */
void makeFibre(Box *box_ptr, PetscInt fID, PetscInt nOnFibre, 
				PetscScalar radius, Node **nList_ptr_ptr);

/**
 * \brief Checks node arguments are all legal
 * \param box_ptr
 * \param nID
 * \param nType
 * \param x
 * \param y
 * \param z
 * \param gIndex_ptr
 * \param gamma
 */
void checkNodeArguments(Box *box_ptr, PetscInt nID, PetscInt nType, 
				PetscScalar x, PetscScalar y, PetscScalar z, PetscInt *gIndex_ptr, PetscScalar gamma);

/**
 * \brief Creates a node within its allocated location in a box
 * \param box_ptr
 * \param nID
 * \param nType
 * \param x
 * \param y
 * \param z
 * \param gIndex_ptr
 * \param gamma
 */
PetscErrorCode makeNode(Box *box_ptr, PetscInt nID, PetscInt nType, 
				PetscScalar x, PetscScalar y, PetscScalar z, PetscInt *gIndex_ptr, PetscScalar gamma);

#endif

