#ifndef SYSTEM_ASSEMBLY_H
#define SYSTEM_ASSEMBLY_H

#include "localAssembly.h"

/**
 * \brief Initiates system assembly routine.
 * \param H Global matrix to be assembled
 * \return Index to represent Petsc error code.
 */
PetscErrorCode systemAssembly(const Box *box_ptr, const Parameters *par_ptr, Mat H, Vec b);

/**
 * \brief Prints out a sample of vector values.
 * \param vecToPrint Vector to be printed.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode printVecValuesSample(Vec vecToPrint);

/**
 * \brief Applies a uniform force to the global matrix 
 * \param H Global matrix to be shifted
 * \param lambda The force representing the stiffness of the elastic medium
 * \param N Number of internal nodes to loop over.
 * \return Index to represent Petsc error code
 */
PetscErrorCode applyEMToDecoupledMatrix(Mat H, PetscScalar lambda, PetscInt N);

/**
 * \brief Applies a force to the global matrix with varied weight on the diagonal. 
 * The weighting is determined based on the number of nodes in each couple.
 * \param H Global matrix to be shifted
 * \param lambda The force representing the stiffness of the elastic medium
 * \param coupleCount Number of couples to loop over.
 * \param coupleList An array of pointers to all the couples.
 * \return Index to represent Petsc error code
 */
PetscErrorCode applyEMToCoupledMatrix(Mat H, PetscScalar lambda, PetscInt coupleCount, const Couple *coupleList);

/**
 * \brief Applies a force to the global matrix 
 * \param H Global matrix to be shifted
 * \param lambda The force representing the stiffness of the elastic medium
 * \param internalCount Needed in the case of decoupled systems.
 * \param coupleCount Used as a flag to determine which medium to apply.
 * \param coupleList An array of pointers to all the couples.
 * \return Index to represent Petsc error code
 */
PetscErrorCode applyElasticMediumToMatrix(Mat H, PetscScalar lambda, PetscInt internalCount, PetscInt coupleCount, const Couple *coupleList);

/**
 * \brief Applies a weighted force to the global RHS vector for a decoupled system.
 * \param box The box pointer used to loop over the internal nodes
 * \param B Global vector to be shifted
 * \param lambda The force representing the stiffness of the elastic medium
 * \return Index to represent Petsc error code
 */
PetscErrorCode applyEMToDecoupledRHSVector(const Box *box_ptr, Vec B, PetscScalar lambda);

/**
 * \brief Applies a weighted force to the global RHS vector for a coupled system.
 * \param box The box pointer used to loop over the internal nodes
 * \param B Global vector to be shifted
 * \param lambda The force representing the stiffness of the elastic medium
 * \return Index to represent Petsc error code
 */
PetscErrorCode applyEMToCoupledRHSVector(const Box *box_ptr, Vec B, PetscScalar lambda);

/**
 * \brief Applies a weighted force to the global RHS vector
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
PetscErrorCode assembleAffineDisplacementVector(const Box *box_ptr, Vec U_aff);

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
PetscErrorCode solveAssembledMatrix(const char *rowFile, const char *colFile, const char *matFile, 
									const char *rhsFile, const char *solFile, PetscInt n);

/**
 * \brief Reads in a file of integers to an array
 * \param fileName String representing name of file to be read in.
 * \param array Array of integers to be written.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readInt(const char *fileName, PetscInt *array, PetscInt n);

/**
 * \brief Reads in a file of doubles to an array
 * \param fileName String representing name of file to be read in.
 * \param array Array of doubles to be written.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode readDbl(const char *fileName, PetscScalar *array, PetscInt n);

/**
 * \brief Writes out an array of doubles to file
 * \param fileName String representing name of file to be written to.
 * \param array Array of doubles to read from.
 * \param n Length of array to be read in.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode writeDbl(const char *fileName, PetscScalar *array, PetscInt n) ;

#endif
