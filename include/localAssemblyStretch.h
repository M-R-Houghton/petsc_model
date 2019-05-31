#ifndef LOCAL_ASSEMBLY_STRETCH_H
#define LOCAL_ASSEMBLY_STRETCH_H

#include "globalAssemblyBend.h"

/**
 * \brief Validates that a given segment length is permitted
 * \param segLength
 * \param xyzPer
 * \param xyzDim
 */
void checkSegLength(PetscScalar segLength, const PetscInt *xyzPer, const PetscScalar *xyzDim);

/**
 * \brief Calculates the stretching constant k
 * \param radius
 * \param youngsModulus
 * \param segLength
 * \return
 */
PetscScalar calculateK(PetscScalar radius, PetscScalar youngsModulus, PetscScalar segLength);

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
PetscErrorCode addFibreLocalStretch(const Box *box_ptr, const Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B, PetscInt fIndex);

/**
 * \brief Assembles the 2D local stretch matrix of a given pair
 * \param k
 * \param tangVec_ptr
 * \param localStretchMat_A
 */
PetscErrorCode make2DStretchMat(PetscScalar k, const PetscScalar *tangVec, PetscScalar localStretchMat_A[6][6]);

/**
 * \brief Assembles the 2D local stretch RHS vector of a given pair
 * \param u_alphBeta
 * \param k
 * \param tangVec
 * \param localStretchVec_b
 */
PetscErrorCode make2DStretchVec(const PetscScalar *u_alphBeta, PetscScalar k, 
                                const PetscScalar *tangVec, PetscScalar *localStretchVec_b);

/**
 * \brief Assembles the 3D local stretch matrix of a given pair
 * \param k
 * \param tangVec
 * \param localStretchMat_A
 */
PetscErrorCode make3DStretchMat(PetscScalar k, const PetscScalar *tangVec, PetscScalar localStretchMat_A[6][6]);

/**
 * \brief Assembles the 3D local stretch RHS vector of a given pair
 * \param u_alphBeta
 * \param k
 * \param tangVec
 * \param localStretchVec_b
 */
PetscErrorCode make3DStretchVec( const PetscScalar *u_alphBeta, PetscScalar k, 
                                    const PetscScalar *tangVec, PetscScalar *localStretchVec_b );


#endif
