#include "systemAssembly.h"

int systemAssembly(Mat H)
{
	PetscErrorCode ierr;
	PetscInt       i,n = 10,col[3];
	PetscScalar    value[3];

	

	/*
	  Assemble matrix
	*/
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Assembling system...\n");CHKERRQ(ierr);
	value[0] = -1.0; value[1] = 2.0; value[2] = -1.0;
	for(i=1; i<n-1; i++) 
	{
		col[0] = i-1; col[1] = i; col[2] = i+1;
		ierr   = MatSetValues(H,1,&i,3,col,value,INSERT_VALUES);CHKERRQ(ierr);
	}
	i    = n - 1; col[0] = n - 2; col[1] = n - 1;
	ierr = MatSetValues(H,1,&i,2,col,value,INSERT_VALUES);CHKERRQ(ierr);
	i    = 0; col[0] = 0; col[1] = 1; value[0] = 2.0; value[1] = -1.0;
	ierr = MatSetValues(H,1,&i,2,col,value,INSERT_VALUES);CHKERRQ(ierr);
	ierr = MatAssemblyBegin(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

	ierr = MatView(H,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	return 0;
}