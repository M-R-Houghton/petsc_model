#ifndef LOCAL_ASSEMBLY_STRETCH_H
#define LOCAL_ASSEMBLY_STRETCH_H

#include "globalAssemblyBend.h"

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
 * \brief Calculates information about the current fibre segment pair 
 * \param box_ptr
 * \param par_ptr
 * \param s_alph
 * \param s_beta
 * \param k
 * \param t_alphBeta
 * \param fIndex
 * \return Petsc error code
 */
PetscErrorCode calculateSegPairInfo( Box *box_ptr, Parameters *par_ptr, PetscScalar *s_alph, PetscScalar *s_beta,
        PetscScalar *k, PetscScalar *t_alphBeta, PetscInt fIndex );

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
PetscErrorCode make2DStretchMat(PetscScalar k, PetscScalar *tangVec, PetscScalar localStretchMat_A[6][6]);

/**
 * \brief Assembles the 2D local stretch RHS vector of a given pair
 * \param u_alph
 * \param u_beta
 * \param k
 * \param tangVec_ptr
 * \param localStretchVec_b
 */
PetscErrorCode make2DStretchVec(PetscScalar *u_alph, PetscScalar *u_beta, PetscScalar k, PetscScalar *tangVec, PetscScalar *localStretchVec_b);

/**
 * \brief Assembles the 3D local stretch matrix of a given pair
 * \param k
 * \param tangVec
 * \param localStretchMat_A
 */
PetscErrorCode make3DStretchMat(PetscScalar k, PetscScalar *tangVec, PetscScalar localStretchMat_A[6][6]);

/**
 * \brief Assembles the 3D local stretch RHS vector of a given pair
 * \param u_alph
 * \param u_beta
 * \param k
 * \param tangVec
 * \param localStretchVec_b
 */
PetscErrorCode make3DStretchVec(PetscScalar *u_alph, PetscScalar *u_beta, PetscScalar k, PetscScalar *tangVec, PetscScalar *localStretchVec_b);

/**
 * \brief Shifts the diagonal of the 3D local stretch matrix by a small value lambda
 * \param localStretchMat_A
 * \param lambda
 */
PetscErrorCode applyMediumTo3DStretchMat(PetscScalar localStretchMat_A[6][6], const PetscScalar lambda);

/**
 * \brief Shifts the 3D local stretch vector affinely
 * \param localStretchVec_b
 * \param lambda
 * \param u_aff_alph 
 * \param u_aff_beta 
 */
PetscErrorCode applyMediumTo3DStretchVec(PetscScalar localStretchVec_b[6], const PetscScalar lambda, 
                                            const PetscScalar *u_aff_alph, const PetscScalar *u_aff_beta);

#endif
