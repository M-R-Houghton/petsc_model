#include "localAssemblyStretch.h"

/* Checks for legal fibre index and segment length */
void checkKArguments(Box *box_ptr, PetscInt fIndex, PetscScalar segLength)
{
	assert(fIndex >= 0);
	assert(segLength >= 0);
	assert(segLength < box_ptr->xyzDimension[0]*0.5);
	assert(segLength < box_ptr->xyzDimension[1]*0.5);
	assert(segLength < box_ptr->xyzDimension[2]*0.5);
}


/* Calculates the stretching constant k */
PetscScalar calculateK(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex, PetscScalar segLength)
{
	/* validate arguments */
	checkKArguments(box_ptr, fIndex, segLength);

	PetscScalar radius 	= box_ptr->masterFibreList[fIndex].radius;
    PetscScalar yMod 	= par_ptr->youngsModulus;
    PetscScalar area 	= M_PI * pow(radius, 2);
    PetscScalar mu 		= area * yMod;

    return mu / segLength;
}


/* Adds local stretch information for a single fibre to global system */
PetscErrorCode addFibreLocalStretch(Box *box_ptr, Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B, PetscInt fIndex)
{
	PetscErrorCode ierr = 0;

	Fibre *fibre_ptr = &(box_ptr->masterFibreList[fIndex]);

	/* setup local matrix and rhs vector */

	/* setup static vectors */

	/* loop over every pair of nodes on the fibre */
	int i;
	for (i = 0; i < fibre_ptr->nodesOnFibre; i++)
	{


		if (DIMENSION == 2)
		{
			/* assemble the 2D local matrix and rhs vector */
			//ierr = make2DStretchMatrix(k, tangVec_ptr, localStretchMat_A);CHKERRQ(ierr);
			//ierr = make2DStretchVec(alpha_ptr, beta_ptr, k, tangVec_ptr, localStretchVec_b);CHKERRQ(ierr);
		}
		else if (DIMENSION == 3)
		{
			/* assemble the 3D local matrix and rhs vector */
			//ierr = make3DStretchMatrix(k, tangVec_ptr, localStretchMat_A);CHKERRQ(ierr);
			//ierr = make3DStretchVec(alpha_ptr, beta_ptr, k, tangVec_ptr, localStretchVec_b);CHKERRQ(ierr);
		}

		/* determine contributions and add to the global system */
	}


	return ierr;
}


/* Assembles the local stretch matrix of a given pair */
PetscErrorCode make3DStretchMatrix(PetscScalar k, PetscScalar *tangVec_ptr, Mat localStretchMat_A)
{
	PetscErrorCode ierr = 0;
	PetscInt       i,n = 10,col[3];
	PetscScalar    value[3];

	value[0] = -1.0; value[1] = 2.0; value[2] = -1.0;
	for(i=1; i<n-1; i++) 
	{
		col[0] = i-1; col[1] = i; col[2] = i+1;
		ierr   = MatSetValues(localStretchMat_A,1,&i,3,col,value,INSERT_VALUES);CHKERRQ(ierr);
	}
	i    = n - 1; col[0] = n - 2; col[1] = n - 1;
	ierr = MatSetValues(localStretchMat_A,1,&i,2,col,value,INSERT_VALUES);CHKERRQ(ierr);
	i    = 0; col[0] = 0; col[1] = 1; value[0] = 2.0; value[1] = -1.0;
	ierr = MatSetValues(localStretchMat_A,1,&i,2,col,value,INSERT_VALUES);CHKERRQ(ierr);
	ierr = MatAssemblyBegin(localStretchMat_A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(localStretchMat_A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

	ierr = MatView(localStretchMat_A,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	return ierr;
}


/* Assembles the local stretch RHS vector of a given pair */
PetscErrorCode makeStretchVec( Node *alpha_ptr, Node *beta_ptr, PetscScalar k, 
								PetscScalar *tangVec_ptr, Vec localStretchVec_b )
{
	PetscErrorCode ierr = 0;

	return ierr;
}

