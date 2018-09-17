#include "systemAssemblyTests.h"

/* Tests system assembly routine */
PetscErrorCode test_systemAssembly()
{
	PetscErrorCode 	ierr;
	Box				*box_ptr;
	Parameters		*par_ptr;
	Vec             dummy_b;
	Mat 			dummy_H;
	PetscInt		n = 10;

	/* vector set up */
	ierr = VecCreate(PETSC_COMM_WORLD,&dummy_b);CHKERRQ(ierr);
	ierr = VecSetSizes(dummy_b,PETSC_DECIDE,n);CHKERRQ(ierr);
	ierr = VecSetFromOptions(dummy_b);CHKERRQ(ierr);

	/* matrix set up */
	ierr = MatCreate(PETSC_COMM_WORLD,&dummy_H);CHKERRQ(ierr);
	ierr = MatSetSizes(dummy_H,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);
	ierr = MatSetFromOptions(dummy_H);CHKERRQ(ierr);
	ierr = MatSetUp(dummy_H);CHKERRQ(ierr);

	/* assembly tests go here */
	//ierr = systemAssembly(box_ptr, par_ptr, dummy_H,dummy_b);CHKERRQ(ierr);

	/* clean up */
	ierr = VecDestroy(&dummy_b);CHKERRQ(ierr); 
	ierr = MatDestroy(&dummy_H);CHKERRQ(ierr);
	return ierr;
}