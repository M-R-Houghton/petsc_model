//#include <criterion/criterion.h>
//#include "localAssemblyStretch.h"
#include "unitTestFixtures.h"

Test(testCalculateK, testOutputKValue)
{
	PetscScalar tol 					= 1e-12;
	Parameters *par_ptr 				= (Parameters *)malloc(sizeof(Parameters));
	par_ptr->youngsModulus 				= 1.0/(M_PI * pow(0.01, 2));

	PetscInt fCount 		   			= 2;
	Box *box_ptr 			   			= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0]			= 10;
	box_ptr->xyzDimension[1]			= 10;
	box_ptr->xyzDimension[2]			= 10;
	box_ptr->fibreCount 	   			= fCount;
	box_ptr->masterFibreList   			= (Fibre*)calloc(fCount, sizeof(Fibre));
	box_ptr->masterFibreList[0].fibreID = 0;
	box_ptr->masterFibreList[0].radius 	= 0.01;
	box_ptr->masterFibreList[1].fibreID = 1;
	box_ptr->masterFibreList[1].radius 	= 0.3;

	cr_expect_float_eq(calculateK(box_ptr, par_ptr, 0, 0.25), 4.0, 		tol);
	cr_expect_float_eq(calculateK(box_ptr, par_ptr, 0, 0.50), 2.0, 		tol);
	cr_expect_float_eq(calculateK(box_ptr, par_ptr, 0, 0.75), 1.0/0.75, tol);
	cr_expect_float_eq(calculateK(box_ptr, par_ptr, 0, 1.00), 1.0, 		tol);

	cr_expect_float_eq(calculateK(box_ptr, par_ptr, 1, 0.25), 900.0/0.25, tol);
	cr_expect_float_eq(calculateK(box_ptr, par_ptr, 1, 0.50), 900.0/0.50, tol);
	cr_expect_float_eq(calculateK(box_ptr, par_ptr, 1, 0.75), 900.0/0.75, tol);
	cr_expect_float_eq(calculateK(box_ptr, par_ptr, 1, 1.00), 900.0, 	  tol);

	/* clean up */
	free(par_ptr); par_ptr = NULL;
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
}


Test(testMakeStretchMatrix, testOutputValues, .init = setup, .fini = teardown)
{
	cr_skip_test();
	/*
	PetscErrorCode 	ierr;
	PetscScalar 	k = 0.0;
	PetscScalar 	tangVec[3] = {1.0, 2.0, 3.0};
	Mat 			A;
	PetscInt       	n = 10;
	PetscBool      	nonzeroguess = PETSC_FALSE;
	//ierr = PetscOptionsGetInt(NULL,NULL,"-n",&n,NULL);
	//ierr = PetscOptionsGetBool(NULL,NULL,"-nonzero_guess",&nonzeroguess,NULL);

	ierr = MatCreate(PETSC_COMM_WORLD,&A);
	//ierr = MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,n,n);
	//ierr = MatSetFromOptions(A);
	//ierr = MatSetUp(A);

	//cr_expect(makeStretchMatrix(k, tangVec, A) == 0);

	ierr = MatDestroy(&A);
	*/
}