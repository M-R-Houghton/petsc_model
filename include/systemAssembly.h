#ifndef SYSTEM_ASSEMBLY_H
#define SYSTEM_ASSEMBLY_H

#include "localAssembly.h"

/**
 * \brief Initiates system assembly routine.
 * \param H Global matrix to be assembled
 * \return Index to represent Petsc error code.
 */
PetscErrorCode systemAssembly(Box *box_ptr, Parameters *par_ptr, Mat H, Vec b);

/**
 * \brief Applies a uniform force to the global matrix 
 * \param H Global matrix to be shifted
 * \param lambda The force representing the stiffness of the elastic medium
 * \return Index to represent Petsc error code
 */
PetscErrorCode applyEMToDecoupledMatrix(Mat H, PetscScalar lambda);

/**
 * \brief Applies a force to the global matrix with varied weight on the diagonal. 
 * The weighting is determined based on the number of nodes in each couple.
 * \param H Global matrix to be shifted
 * \param lambda The force representing the stiffness of the elastic medium
 * \return Index to represent Petsc error code
 */
PetscErrorCode applyEMToCoupledMatrix(Mat H, PetscScalar lambda);

/**
 * \brief Applies a force to the global matrix 
 * \param H Global matrix to be shifted
 * \param lambda The force representing the stiffness of the elastic medium
 * \param coupleCount Used as a flag to determine which medium to apply.
 * \return Index to represent Petsc error code
 */
PetscErrorCode applyElasticMediumToMatrix(Mat H, PetscScalar lambda, PetscInt coupleCount);

/**
 * \brief Applies a uniform force to the global RHS vector
 * \param box The box pointer used to loop over the internal nodes
 * \param B Global vector to be shifted
 * \param lambda The force representing the stiffness of the elastic medium
 * \return Index to represent Petsc error code
 */
PetscErrorCode applyElasticMediumToRHSVector(const Box *box_ptr, Vec B, PetscScalar lambda);

/**
 * \brief Applies a uniform force such that the network behaves suspended in an elastic medium 
 * \param box The box pointer used to loop over the internal nodes
 * \param H Global matrix to be shifted
 * \param B Global vector to be shifted
 * \param lambda The force representing the stiffness of the elastic medium
 * \return Index to represent Petsc error code
 */
PetscErrorCode applyElasticMedium(const Box *box_ptr, Mat H, Vec B, PetscScalar lambda);

/**
 * \brief Assembles a global vector from the affine displacements of every internal node 
 * \param box The box pointer used to loop over the internal nodes
 * \param U_aff The global displacement vector to be assembled
 * \return Index to represent Petsc error code
 */
PetscErrorCode assembleAffineDisplacementVector(Box *box_ptr, Vec U_aff);

/**
 * \brief Solves a matrix from pre-assembled arrays.
 * \param rowFile String representing name of row pointer file to be read in.
 * \param colFile String representing name of column index file to be read in.
 * \param matFile String representing name of matrix value file to be read in.
 * \param vecFile String representing name of RHS vector file to be read in.
 * \param solFile String representing name of solution vector file to be read in.
 * \param n Dimension of global matrix (nxn).
 * \return Index to represent Petsc error code.
 */
PetscErrorCode solveAssembledMatrix(char const *rowFile, char const *colFile, char const *matFile, 
									char const *rhsFile, char const *solFile, PetscInt n);

/**
 * \brief Reads in a file of integers to an array
 * \param fileName String representing name of file to be read in.
 * \param array Array of integers to be written.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readInt(char const *fileName, PetscInt *array, PetscInt n);

/**
 * \brief Reads in a file of doubles to an array
 * \param fileName String representing name of file to be read in.
 * \param array Array of doubles to be written.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readDbl(char const *fileName, PetscScalar *array, PetscInt n);

/**
 * \brief Writes out an array of doubles to file
 * \param fileName String representing name of file to be written to.
 * \param array Array of doubles to read from.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode writeDbl(char const *fileName, PetscScalar *array, PetscInt n) ;

#endif
