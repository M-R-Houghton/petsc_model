#include "systemAssembly.h"

/* Initiates system assembly routine */
PetscErrorCode systemAssembly(Mat H, Vec b)
{
	PetscErrorCode ierr;
	PetscInt       i,n = 10,col[3];
	PetscScalar    value[3];

	/*
	 * Assemble matrix
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
	return ierr;
}


/* Assembles tripod matrix */
PetscErrorCode tripodAssembly()
{
	PetscErrorCode 	ierr;
	Mat            	H;
	PetscInt 		n=3;

	PetscInt 		idxm[9];
	PetscInt 		idxn[9];
	PetscScalar 	value[9];

	char rowFileName[100] = "data/row/idxm.lmbTripod1";
    readInt(rowFileName, idxm, 9);
    char colFileName[100] = "data/col/col.lmbTripod1";
    readInt(colFileName, idxn, 9);
    char matFileName[100] = "data/mat/mat.lmbTripod1";
    readDbl(matFileName, value, 9);

	ierr = MatCreate(PETSC_COMM_WORLD,&H);CHKERRQ(ierr);
	ierr = MatSetSizes(H,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);

	//ierr = MatSetFromOptions(H);CHKERRQ(ierr);
	ierr = MatSetUp(H);CHKERRQ(ierr);

	ierr = MatSetValues(H,1,idxm,3,idxn,value,INSERT_VALUES);CHKERRQ(ierr);		/* produces 1st row where 1st row should be */
	ierr = MatSetValues(H,2,idxm,3,idxn,value,INSERT_VALUES);CHKERRQ(ierr);		/* produces 2nd row where 1st row should be */
	ierr = MatSetValues(H,3,idxm,3,idxn,value,INSERT_VALUES);CHKERRQ(ierr);		/* produces 3rd row where 1st row should be */

	ierr = MatAssemblyBegin(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

	ierr = MatView(H,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	ierr = MatDestroy(&H);CHKERRQ(ierr);

	return ierr;
}


/* Reads in a file of integers to an array */
PetscErrorCode readInt(char *fileName, PetscInt *array, PetscInt n)
{
	PetscErrorCode 	ierr;
	PetscInt 		i,inp;

    FILE *fp = fopen( fileName, "r" );
    ierr = PetscPrintf(PETSC_COMM_WORLD, "%s opened successfully\n", fileName);CHKERRQ(ierr);
    for( i=0; i<n; ++i ) 
    {
        fscanf(fp,"%d",&inp);
        array[i] = inp;
    } 
    fclose(fp);
    ierr = PetscPrintf(PETSC_COMM_WORLD, "%s closed successfully\n", fileName);CHKERRQ(ierr);

    return ierr;
}


/* Reads in a file of doubles to an array */
PetscErrorCode readDbl(char *fileName, PetscScalar *array, PetscInt n)
{
	PetscErrorCode 	ierr;
	PetscInt 		i;
    PetscScalar 	inp;

    FILE *fp = fopen( fileName, "r" );
    ierr = PetscPrintf(PETSC_COMM_WORLD, "%s opened successfully\n", fileName);CHKERRQ(ierr);
    for( i=0; i<n; ++i ) 
    {
        fscanf(fp,"%lf",&inp);
        array[i] = inp;
    } 
    fclose(fp);
    ierr = PetscPrintf(PETSC_COMM_WORLD, "%s closed successfully\n", fileName);CHKERRQ(ierr);

    return ierr;
}

