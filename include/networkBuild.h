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
Parameters *makeParameters(const char *input, const char *output, PetscScalar gamma, PetscScalar yMod);

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
 * \param box_ptr Box containing the relevant data to represent the given network.
 * \param fID
 * \param nOnFibre
 * \param radius
 * \param nList_ptr_ptr
 */
void checkFibreArguments(Box *box_ptr, PetscInt fID, PetscInt nOnFibre, 
				PetscScalar radius, Node **nList_ptr_ptr);

/**
 * \brief Creates a fibre within its allocated location in a box
 * \param box_ptr Box containing the relevant data to represent the given network.
 * \param fID
 * \param nOnFibre
 * \param radius
 * \param nList_ptr_ptr
 * \return Index to represent Petsc error code.
 */
PetscErrorCode makeFibre(Box *box_ptr, PetscInt fID, PetscInt nOnFibre, 
							PetscScalar radius, Node **nList_ptr_ptr);

/**
 * \brief Checks node arguments are all legal
 * \param box_ptr Box containing the relevant data to represent the given network.
 * \param nID
 * \param nType
 * \param x
 * \param y
 * \param z
 * \param gIndex_ptr
 * \param gamma
 */
void checkNodeArguments(Box *box_ptr, PetscInt nID, PetscInt nType, 
				PetscScalar x, PetscScalar y, PetscScalar z, PetscScalar gamma);

/**
 * \brief Creates a node within its allocated location in a box
 * \param box_ptr Box containing the relevant data to represent the given network.
 * \param nID
 * \param nType
 * \param x
 * \param y
 * \param z
 * \param gIndex_ptr
 * \param gamma
 * \return Index to represent Petsc error code.
 */
PetscErrorCode makeNode(Box *box_ptr, PetscInt nID, PetscInt nType, 
				PetscScalar x, PetscScalar y, PetscScalar z, PetscInt *gIndex_ptr, PetscScalar gamma);

/**
 * \brief Checks couple arguments are all legal
 * \param box_ptr Box containing the relevant data to represent the given network.
 * \param cID
 * \param nInCouple
 * \param nID1
 * \param nID2
 */
void checkCoupleArguments(Box *box_ptr, PetscInt const cID, PetscInt const nID1, PetscInt const nID2);

/**
 * \brief Creates a couple within its allocated location in a box
 * \param box_ptr Box containing the relevant data to represent the given network.
 * \param cID
 * \param nID1
 * \param nID2
 */
PetscErrorCode makeCouple(Box *box_ptr, PetscInt const cID, PetscInt const nID1, PetscInt const nID2);

#endif

