#ifndef LOCAL_ASSEMBLY_BEND_H
#define LOCAL_ASSEMBLY_BEND_H

#include "localAssemblyStretch.h"

/**
 * \brief Checks for legal kappa calculation arguments
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 */
void checkKappaArguments(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);

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
PetscErrorCode addFibreLocalBend(Box *box_ptr, Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B, PetscInt fIndex);

/**
 * \brief Assembles the local 2D bend matrix of a given triplet
 * \param s_alphOmeg_ptr
 * \param s_omegBeta_ptr
 * \param s_alphBeta_ptr
 * \param bConst
 * \param localBendMat_A
 */
PetscErrorCode make2DBendMat(PetscScalar *s_alphOmeg_ptr, PetscScalar *s_omegBeta_ptr, 
								PetscScalar *s_alphBeta_ptr, PetscScalar bConst, PetscScalar localBendMat_A[9][9] );

/**
 * \brief Assembles the local 2D bend RHS vector of a given triplet
 * \param alph_ptr
 * \param omeg_ptr
 * \param beta_ptr
 * \param s_alphOmeg_ptr
 * \param s_omegBeta_ptr
 * \param s_alphBeta_ptr
 * \param bConst
 * \param localBendVec_b
 */
PetscErrorCode make2DBendVec(Node *alph_ptr, Node *omeg_ptr, Node *beta_ptr,
                    			PetscScalar *s_alphOmeg_ptr, PetscScalar *s_omegBeta_ptr,
                    			PetscScalar *s_alphBeta_ptr, PetscScalar bConst, PetscScalar localBendVec_b[9] );

/**
 * \brief Assembles the local 3D bend matrix of a given triplet
 * \param s_alphOmeg_ptr
 * \param s_omegBeta_ptr
 * \param s_alphBeta_ptr
 * \param bConst
 * \param localBendMat_A
 */
PetscErrorCode make3DBendMat(PetscScalar *s_alphOmeg_ptr, PetscScalar *s_omegBeta_ptr, 
								PetscScalar *s_alphBeta_ptr, PetscScalar bConst, PetscScalar localBendMat_A[9][9] );

/**
 * \brief Assembles the local 3D bend RHS vector of a given triplet
 * \param alph_ptr
 * \param omeg_ptr
 * \param beta_ptr
 * \param s_alphOmeg_ptr
 * \param s_omegBeta_ptr
 * \param s_alphBeta_ptr
 * \param bConst
 * \param localBendVec_b
 */
PetscErrorCode make3DBendVec(Node *alph_ptr, Node *omeg_ptr, Node *beta_ptr,
                    			PetscScalar *s_alphOmeg_ptr, PetscScalar *s_omegBeta_ptr,
                    			PetscScalar *s_alphBeta_ptr, PetscScalar bConst, PetscScalar localBendVec_b[9] );




#endif
