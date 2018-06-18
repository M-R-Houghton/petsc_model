#include <petscksp.h>
#include <criterion/criterion.h>
#include "networkBuild.h"

Test(testMakeParameters, testValueAssignment)
{
	PetscScalar gamma = 0.5, yMod = 1.0;

	Parameters *par_ptr = makeParameters(gamma,yMod);
	cr_expect_not_null(par_ptr);

	cr_expect(par_ptr->gamma         == 0.5);
	cr_expect(par_ptr->youngsModulus == 1.0);

	free(par_ptr); par_ptr = NULL;
	cr_expect_null(par_ptr);
}


Test(testMakeSparse, testValueAssignment)
{
	PetscInt n = 25, nz = 15;
	
	Sparse *sparse_ptr = makeSparse(n,nz);
	cr_expect_not_null(sparse_ptr);
	cr_expect(sparse_ptr->n  == 25);
	cr_expect(sparse_ptr->nz == 15);			/* test inputs */

	sparse_ptr->col[14] = 82;
	cr_expect(sparse_ptr->col[14] == 82);		/* test assignment */

	/* clean up */
	free(sparse_ptr->counter); sparse_ptr->counter = NULL;
	free(sparse_ptr->rowp); sparse_ptr->rowp = NULL;
	free(sparse_ptr->col); sparse_ptr->col = NULL;
	free(sparse_ptr->mat); sparse_ptr->mat = NULL;
	free(sparse_ptr); sparse_ptr = NULL;
	cr_expect_null(sparse_ptr);
}


Test(testCheckBoxArguments, testValidArguments)
{
	cr_expect(checkBoxArguments(1,1,1,1,1,1,1,1) == 0);
}
