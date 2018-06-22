#ifndef LOCAL_ASSEMBLY_BEND_H
#define LOCAL_ASSEMBLY_BEND_H

#include "localAssemblyStretch.h"

/**
 * \brief Calculates the bending term kappa
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscScalar calculateKappa(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);

/**
 * \brief Adds local bend information for a single fibre to global system
 * Loops through a fibre in triplets
 * \param box_ptr
 * \param par_ptr
 * \param globalMat_H
 * \param globalVec_B
 * \param fIndex
 * \return
 */
PetscInt addFibreLocalBend(Box *box_ptr, Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B, PetscInt fIndex);

/**
 * \brief Assembles the local bend matrix of a given triplet
 * \param s_alphaOmega_ptr
 * \param s_omegaBeta_ptr
 * \param s_alphaBeta_ptr
 * \param bConst
 * \param localBendMat_A
 */
PetscErrorCode makeBendMatrix(PetscScalar *s_alphaOmega_ptr, PetscScalar *s_omegaBeta_ptr, PetscScalar *s_alphaBeta_ptr, 
								PetscScalar bConst, Mat localBendMat_A);

/**
 * \brief Assembles the local bend RHS vector of a given triplet
 * \param alpha_ptr
 * \param omega_ptr
 * \param beta_ptr
 * \param s_alphaOmega_ptr
 * \param s_omegaBeta_ptr
 * \param s_alphaBeta_ptr
 * \param bConst
 * \param localBendVec_b
 */
PetscErrorCode makeBendVec(Node *alpha_ptr, Node *omega_ptr, Node *beta_ptr,
                    PetscScalar *s_alphaOmega_ptr, PetscScalar *s_omegaBeta_ptr,
                    PetscScalar *s_alphaBeta_ptr, PetscScalar bConst, Vec *localBendVec_b);


#endif
