#ifndef GLOBAL_ASSEMBLY_BEND_H
#define GLOBAL_ASSEMBLY_BEND_H

#include "globalAssemblyStretch.h"

/**
 * \brief Checks bend matrix contribution indexes are all legal 
 * \param gIdx_A The global index for node_A.
 * \param gIdx_B The global index for node_B.
 * \param lIdx_A The local index for node_A.
 * \param lIdx_B The local index for node_B.
 */
void checkMatBendContIndexes( PetscInt gIdx_A, PetscInt gIdx_B, PetscInt lIdx_A, PetscInt lIdx_B );

/**
 * \brief Adds a single bending contribution of 9 values to the global matrix
 * Extracts 9 values from a local stretch matrix and adds them to the global using global indexes 
 * \param globalMat_H The global matrix H.
 * \param localMat The local matrix to extract values from.
 * \param N Total number of internal nodes.
 * \param gIdx_A The global index for node_A.
 * \param gIdx_B The global index for node_B.
 * \param lIdx_A The local index for node_A.
 * \param lIdx_B The local index for node_B.
 * \return Petsc error code.
 */
PetscErrorCode addMatSingleBendContFAST( Mat globalMat_H, const PetscScalar localMat[][9], PetscInt N,
										PetscInt gIdx_A, PetscInt gIdx_B, PetscInt lIdx_A, PetscInt lIdx_B );


PetscErrorCode addMatSingleBendCont( Mat globalMat_H, const PetscScalar localMat[][9], PetscInt N,
										PetscInt gIdx_A, PetscInt gIdx_B, PetscInt lIdx_A, PetscInt lIdx_B );

/**
 * \brief Checks bend vector contribution indexes are all legal 
 * \param gIdx_A The global index for node_A.
 * \param lIdx_A The local index for node_A.
 */
void checkVecBendContIndexes( PetscInt gIdx_A, PetscInt lIdx_A );

/**
 * \brief Adds a single bending contribution of 3 values to the global vector
 * Extracts 3 values from a local Bend vector and adds them to the global using global indexes 
 * \param globalVec_H The global vector H.
 * \param localVec The local vector to extract values from.
 * \param N Total number of internal nodes.
 * \param gIdx_A The global index for node_A.
 * \param lIdx_A The local index for node_A.
 * \return Petsc error code.
 */
PetscErrorCode addVecSingleBendCont( Vec globalVec_B, const PetscScalar localVec[], PetscInt N,
										PetscInt gIdx_A, PetscInt lIdx_A );

/**
 * \brief Adds local bending contributions to the global matrix and RHS vector
 * \param globalMat_H The global matrix H.
 * \param globalVec_B The global rhs vector B.
 * \param N Total number of internal nodes.
 * \param localMat The local matrix.
 * \param localVec The local rhs vector.
 * \param alph_ptr A pointer to the node alpha.
 * \param omeg_ptr A pointer to the node omega.
 * \param beta_ptr A pointer to the node beta.
 * \return Petsc error code.
 */
PetscErrorCode addBendContToGlobal( Mat globalMat_H, Vec globalVec_B, PetscInt N,
									const PetscScalar localMat[][9], const PetscScalar localVec[],
									const Node *alph_ptr, const Node *omeg_ptr, const Node *beta_ptr );


#endif
