#ifndef GLOBAL_ASSEMBLY_STRETCH_H
#define GLOBAL_ASSEMBLY_STRETCH_H

#include "networkRead.h"

/**
 * \brief Adds a single stretching contribution of 9 values to the global matrix
 * Extracts 9 values from a local stretch matrix and adds them to the global using global indexes
 * This function should be faster than the standard version, due to more efficient adding
 * \param box_ptr Pointer to box containing internal node count.
 * \param globalMat_H The global matrix H.
 * \param localMat The local matrix to extract values from.
 * \param gInd_A The global index for node alpha.
 * \param gInd_B The global index for node beta.
 * \param lInd_A The local index for node alpha.
 * \param lInd_B The local index for node beta.
 * \return Petsc error code.
 */
PetscErrorCode addSingleStretchContFAST( Box *box_ptr, Mat globalMat_H, PetscScalar localMat[][6], 
											PetscInt gInd_A, PetscInt gInd_B, PetscInt lInd_A, PetscInt lInd_B );

/**
 * \brief Adds a single stretching contribution of 9 values to the global matrix
 * Extracts 9 values from a local stretch matrix and adds them to the global using global indexes
 * \param box_ptr Pointer to box containing internal node count.
 * \param globalMat_H The global matrix H.
 * \param localMat The local matrix to extract values from.
 * \param gInd_A The global index for node alpha.
 * \param gInd_B The global index for node beta.
 * \param lInd_A The local index for node alpha.
 * \param lInd_B The local index for node beta.
 * \return Petsc error code.
 */
PetscErrorCode addSingleStretchCont( Box *box_ptr, Mat globalMat_H, PetscScalar localMat[][6], 
										PetscInt gInd_A, PetscInt gInd_B, PetscInt lInd_A, PetscInt lInd_B );

/**
 * \brief Adds local stretching contributions to the global matrix and RHS vector
 * \param alpha_ptr A pointer to x the node alpha.
 * \param beta_ptr A pointer to x the node beta.
 * \param globalMat_H The global matrix H.
 * \param globalVec_B The global rhs vector B.
 * \param localMat The local matrix.
 * \param localVec The local rhs vector.
 * \return Petsc error code.
 */
PetscErrorCode addStretchContToGlobal( Node *alpha_ptr, Node *beta_ptr,
										Mat globalMat_H, Vec globalVec_B, 
									   	PetscScalar localMat[][6], PetscScalar localVec[] );

#endif