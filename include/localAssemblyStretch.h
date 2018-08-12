#ifndef LOCAL_ASSEMBLY_STRETCH_H
#define LOCAL_ASSEMBLY_STRETCH_H

#include "networkRead.h"

/**
 * \brief Checks for legal fibre index and segment length
 * \param box_ptr
 * \param fIndex
 * \param segLength
 */
void checkKArguments(Box *box_ptr, PetscInt fIndex, PetscScalar segLength);

/**
 * \brief Calculates the stretching constant k
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \param segLength
 * \return
 */
PetscScalar calculateK(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex, PetscScalar segLength);

/**
 * \brief Adds local stretch information for a single fibre to global system
 * Loops through a fibre in pairs
 * \param box_ptr
 * \param par_ptr
 * \param globalMat_H
 * \param globalVec_B
 * \param fIndex
 * \return
 */
PetscErrorCode addFibreLocalStretch(Box *box_ptr, Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B, PetscInt fIndex);

/**
 * \brief Assembles the 2D local stretch matrix of a given pair
 * \param k
 * \param tangVec_ptr
 * \param localStretchMat_A
 */
PetscErrorCode make2DStretchMatrix(PetscScalar k, PetscScalar *tangVec_ptr, Mat localStretchMat_A);

/**
 * \brief Assembles the 3D local stretch matrix of a given pair
 * \param k
 * \param tangVec_ptr
 * \param localStretchMat_A
 */
PetscErrorCode make3DStretchMatrix(PetscScalar k, PetscScalar *tangVec_ptr, Mat localStretchMat_A);

/**
 * \brief Assembles the local stretch RHS vector of a given pair
 * \param alpha_ptr
 * \param beta_ptr
 * \param k
 * \param tangVec_ptr
 * \param localStretchVec_b
 */
PetscErrorCode makeStretchVec(Node *alpha_ptr, Node *beta_ptr, PetscScalar k, PetscScalar *tangVec_ptr, Vec localStretchVec_b);


#endif