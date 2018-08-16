#include "networkDestroyTests.h"

/* Tests network destroy routine */
PetscErrorCode test_networkDestroy()
{
	PetscErrorCode ierr;

	ierr = networkDestroy();CHKERRQ(ierr);

	ierr = test_destroyParameters();CHKERRQ(ierr);
	ierr = test_destroySparse();CHKERRQ(ierr);
	ierr = test_destroyBox();CHKERRQ(ierr);
	
	return ierr;
}

/* Tests parameter destroyer */
PetscErrorCode test_destroyParameters()
{
	PetscErrorCode ierr;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] destroyParameters...\n");CHKERRQ(ierr);

	Parameters *par_ptr 	= (Parameters *)malloc(sizeof(Parameters));
	par_ptr->gamma 			= 0.5;
	par_ptr->youngsModulus 	= 1.0;

	destroyParameters(par_ptr);		/* test that internal asserts check out */
	
	return ierr;
}

/* Tests sparse matrix destroyer */
PetscErrorCode test_destroySparse()
{
	PetscErrorCode ierr;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] destroySparse...\n");CHKERRQ(ierr);

	Sparse *sparse_ptr  = (Sparse *)malloc(sizeof(Sparse));
	sparse_ptr->n 		= 25;
	sparse_ptr->nz 		= 15;
	sparse_ptr->counter = (PetscInt *)calloc(sparse_ptr->n,sizeof(PetscInt));
	sparse_ptr->rowp    = (PetscInt *)calloc(sparse_ptr->n+1,sizeof(PetscInt));
	sparse_ptr->col 	= (PetscInt *)calloc(sparse_ptr->nz,sizeof(PetscInt));
	sparse_ptr->mat 	= (PetscScalar *)calloc(sparse_ptr->nz,sizeof(PetscScalar));

	destroySparse(sparse_ptr);		/* test that internal asserts check out */
	
	return ierr;
}

/* Tests box destroyer */
PetscErrorCode test_destroyBox()
{
	PetscErrorCode ierr;
	PetscInt 	   nCount = 5, fCount = 2;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] destroyBox...\n");CHKERRQ(ierr);

	Box *box_ptr 			   = (Box *)malloc(sizeof(Box));
	box_ptr->nodeCount 		   = nCount;
	box_ptr->nodeInternalCount = -1;
	box_ptr->fibreCount 	   = fCount;
	box_ptr->xyzDimension[0]   = 1;
	box_ptr->xyzDimension[1]   = 2;
	box_ptr->xyzDimension[2]   = 3;
	box_ptr->xyzPeriodic[0]    = 1;
	box_ptr->xyzPeriodic[1]    = 1;
	box_ptr->xyzPeriodic[2]    = 1;

	box_ptr->masterNodeList    = (Node*)calloc(nCount, sizeof(Node));
	box_ptr->masterFibreList   = (Fibre*)calloc(fCount, sizeof(Fibre));

	destroyBox(box_ptr);		/* test that internal asserts check out */
	
	return ierr;
}

