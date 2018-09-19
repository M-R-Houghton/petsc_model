#ifndef GLOBAL_ASSEMBLY_STRETCH_H
#define GLOBAL_ASSEMBLY_STRETCH_H

#include "networkRead.h"

/**
 * \brief Checks stretch matrix contribution indexes are all legal 
 * \param gInd_A The global index for node_A.
 * \param gInd_B The global index for node_B.
 * \param lInd_A The local index for node_A.
 * \param lInd_B The local index for node_B.
 */
void checkMatStretchContIndexes( PetscInt gInd_A, PetscInt gInd_B, PetscInt lInd_A, PetscInt lInd_B );

/**
 * \brief Adds a single stretching contribution of 9 values to the global matrix
 * Extracts 9 values from a local stretch matrix and adds them to the global using global indexes
 * This function should be faster than the standard version, due to more efficient adding 
 * \param globalMat_H The global matrix H.
 * \param localMat The local matrix to extract values from.
 * \param N Total number of internal nodes.
 * \param gInd_A The global index for node_A.
 * \param gInd_B The global index for node_B.
 * \param lInd_A The local index for node_A.
 * \param lInd_B The local index for node_B.
 * \return Petsc error code.
 */
PetscErrorCode addMatSingleStretchContFAST( Mat globalMat_H, PetscScalar localMat[][6], PetscInt N,
											PetscInt gInd_A, PetscInt gInd_B, PetscInt lInd_A, PetscInt lInd_B );

/**
 * \brief Adds a single stretching contribution of 9 values to the global matrix
 * Extracts 9 values from a local stretch matrix and adds them to the global using global indexes 
 * \param globalMat_H The global matrix H.
 * \param localMat The local matrix to extract values from.
 * \param N Total number of internal nodes.
 * \param gInd_A The global index for node_A.
 * \param gInd_B The global index for node_B.
 * \param lInd_A The local index for node_A.
 * \param lInd_B The local index for node_B.
 * \return Petsc error code.
 */
PetscErrorCode addMatSingleStretchCont( Mat globalMat_H, PetscScalar localMat[][6], PetscInt N,
										PetscInt gInd_A, PetscInt gInd_B, PetscInt lInd_A, PetscInt lInd_B );

/**
 * \brief Checks stretch vector contribution indexes are all legal 
 * \param gInd_A The global index for node_A.
 * \param lInd_A The local index for node_A.
 */
void checkVecStretchContIndexes( PetscInt gInd_A, PetscInt lInd_A );

/**
 * \brief Adds a single stretching contribution of 3 values to the global vector
 * Extracts 3 values from a local stretch vector and adds them to the global using global indexes 
 * \param globalVec_H The global vector H.
 * \param localVec The local vector to extract values from.
 * \param N Total number of internal nodes.
 * \param gInd_A The global index for node_A.
 * \param lInd_A The local index for node_A.
 * \return Petsc error code.
 */
PetscErrorCode addVecSingleStretchCont( Vec globalVec_B, PetscScalar localVec[], PetscInt N,
										PetscInt gInd_A, PetscInt lInd_A );

/**
 * \brief Adds local stretching contributions to the global matrix and RHS vector
 * \param alph_ptr A pointer to x the node alpha.
 * \param beta_ptr A pointer to x the node beta.
 * \param globalMat_H The global matrix H.
 * \param globalVec_B The global rhs vector B.
 * \param N Total number of internal nodes.
 * \param localMat The local matrix.
 * \param localVec The local rhs vector.
 * \return Petsc error code.
 */
PetscErrorCode addStretchContToGlobal( Node *alph_ptr, Node *beta_ptr,
										Mat globalMat_H, Vec globalVec_B, PetscInt N,
									   	PetscScalar localMat[][6], PetscScalar localVec[] );

#endif