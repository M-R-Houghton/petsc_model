#ifndef NETWORK_ANALYSIS_H
#define NETWORK_ANALYSIS_H

#include "networkUpdate.h"

/**
 * \brief Initiates network analysis routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkAnalysis();

/**
 * \brief Calculates the stretch energy of a given fibre segment
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscScalar calculateSegStretchEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
										Node *alph_ptr, Node *beta_ptr );

/**
 * \brief Calculates the bend energy of a given fibre segment
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscScalar calculateSegBendEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
									 Node *alph_ptr, Node *omeg_ptr, Node *beta_ptr );

/**
 * \brief Calculates the stretch energy of a given fibre
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscScalar calculateStretchEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);

/**
 * \brief Calculates the bend energy of a given fibre
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscScalar calculateBendEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);

/**
 * \brief Calculates the energy of an entire network
 * \param box_ptr
 * \param par_ptr
 * \return
 */
PetscErrorCode calculateEnergy(Box *box_ptr, Parameters *par_ptr);

/**
 * \brief Calculates the area of a network
 * \param box_ptr
 * \return
 */
PetscScalar calculateArea(Box *box_ptr);

/**
 * \brief Calculates the volume of a network
 * \param box_ptr
 * \return
 */
PetscScalar calculateVolume(Box *box_ptr);

/**
 * \brief Calculates the aperiodic range of a network
 * \param box_ptr
 * \param i, the index to distinguish x,y or z
 * \return
 */
PetscScalar aperiodicRange(Box *box_ptr, PetscInt i);

/**
 * \brief Calculates the shear modulus of a network
 * \param box_ptr
 * \param par_ptr
 * \return
 */
PetscScalar calculateShearModulus(Box *box_ptr, Parameters *par_ptr);


#endif