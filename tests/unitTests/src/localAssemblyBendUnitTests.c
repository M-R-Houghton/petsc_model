//#include <criterion/criterion.h>
//#include "localAssemblyBend.h"
#include "unitTestFixtures.h"

Test(testCalculateKappa, testOutputKappaValue)
{
	PetscScalar tol 					= 1e-12;
	Parameters *par_ptr 				= (Parameters *)malloc(sizeof(Parameters));
	par_ptr->youngsModulus 				= 1.0/(M_PI * pow(0.01, 2));

	PetscInt fCount 		   			= 2;
	Box *box_ptr 			   			= (Box *)malloc(sizeof(Box));
	box_ptr->fibreCount 	   			= fCount;
	box_ptr->masterFibreList   			= (Fibre*)calloc(fCount, sizeof(Fibre));
	box_ptr->masterFibreList[0].fibreID = 0;
	box_ptr->masterFibreList[0].radius 	= 0.01;
	box_ptr->masterFibreList[1].fibreID = 1;
	box_ptr->masterFibreList[1].radius 	= 0.3;

	cr_expect_float_eq(calculateKappa(box_ptr, par_ptr, 0), 0.000025, tol);
	cr_expect_float_eq(calculateKappa(box_ptr, par_ptr, 1), 20.25, 	  tol);

	/* clean up */
	free(par_ptr); par_ptr = NULL;
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
}