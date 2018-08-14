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
	ierr = MatSetValues(H,1,&i,2,col,value,INSERT_VALUES);CHKERRQ(ierr); /* where &i is an array of i */
	ierr = MatAssemblyBegin(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

	ierr = MatView(H,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	return ierr;
}


/* Assembles tripod matrix */
PetscErrorCode tripodAssembly(char *rowFile, char *colFile, char *matFile, char *vecFile, char *solFile)
{
	PetscErrorCode 	ierr;
	Mat            	H;
	Vec 			B,U;
	KSP            	ksp;          /* linear solver context */
	PC             	pc;           /* preconditioner context */
	PetscInt 		i,n=3;

	PetscInt 		idxm[4];
	PetscInt 		idxn[9];
	PetscScalar 	value[9];
	PetscScalar		rhsVec[3];
	PetscScalar		*solVec;

    ierr = readInt(rowFile, idxm, n+1);CHKERRQ(ierr);
    ierr = readInt(colFile, idxn, idxm[n]);CHKERRQ(ierr);
    ierr = readDbl(matFile, value, idxm[n]);CHKERRQ(ierr);
    ierr = readDbl(vecFile, rhsVec, n);CHKERRQ(ierr);

    /* Vector assembly for existing array */
    ierr = VecCreateSeqWithArray(PETSC_COMM_WORLD,1,n,rhsVec,&B);CHKERRQ(ierr);

    /* Set up solution vector */
    ierr = VecDuplicate(B,&U);CHKERRQ(ierr);
    ierr = VecSet(U,1.0);CHKERRQ(ierr);

    /* Print vector to verify assembly */
    ierr = VecView(B,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

    /* Matrix assembly for existing CSR arrays */
	ierr = MatCreateSeqAIJWithArrays(PETSC_COMM_WORLD,n,n,idxm,idxn,value,&H);CHKERRQ(ierr);

	/* Row at a time manual matrix assembly *//*
	ierr = MatCreate(PETSC_COMM_WORLD,&H);CHKERRQ(ierr);
	ierr = MatSetSizes(H,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);

	ierr = MatSetFromOptions(H);CHKERRQ(ierr);
	ierr = MatSetUp(H);CHKERRQ(ierr);

	for (i = 0; i < 3; i++)
	{
		ierr = MatSetValues(H,1,&i,3,idxn,value,INSERT_VALUES);CHKERRQ(ierr);		// produces 1st row where 1st row should be
		//ierr = MatSetValues(H,2,&i,3,idxn,value,INSERT_VALUES);CHKERRQ(ierr);		// produces 2nd row where 1st row should be
		//ierr = MatSetValues(H,3,&i,3,idxn,value,INSERT_VALUES);CHKERRQ(ierr);		// produces 3rd row where 1st row should be
	}

	ierr = MatAssemblyBegin(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	*//* END manual matrix assembly */

	/* Print matrix to verify assembly */
	ierr = MatView(H,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/* Create solver context */
	ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);

	/* Choose matrix as it's own preconditioning matrix */
	ierr = KSPSetOperators(ksp,H,H);CHKERRQ(ierr);

	/* Set up default options */
	ierr = KSPGetPC(ksp,&pc);CHKERRQ(ierr);
	ierr = PCSetType(pc,PCJACOBI);CHKERRQ(ierr);
	ierr = KSPSetTolerances(ksp,1.e-5,PETSC_DEFAULT,PETSC_DEFAULT,PETSC_DEFAULT);CHKERRQ(ierr);

	/* Set runtime options */
    ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr);

    /* Solve linear system */
	ierr = KSPSolve(ksp,B,U);CHKERRQ(ierr);

	/* View solver info */
	ierr = KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/* Check out array */
    ierr = VecGetArray(U, &solVec);CHKERRQ(ierr);
    /* Write out array */
    ierr = writeDbl(solFile, solVec, n);CHKERRQ(ierr);
    /* Check array back in */
    ierr = VecRestoreArray(U, &solVec);CHKERRQ(ierr);

	/* Clean up */
	ierr = VecDestroy(&B);CHKERRQ(ierr);
	ierr = VecDestroy(&U);CHKERRQ(ierr);
	ierr = MatDestroy(&H);CHKERRQ(ierr);
	ierr = KSPDestroy(&ksp);CHKERRQ(ierr);

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


/* Writes out an array of doubles to file */
PetscErrorCode writeDbl(char *fileName, PetscScalar *array, PetscInt n) 
{
    PetscErrorCode ierr;
    PetscInt i;

    FILE *fp = fopen( fileName, "w" );
    ierr = PetscPrintf(PETSC_COMM_WORLD, "%s opened successfully\n", fileName);CHKERRQ(ierr);
    for( i=0; i<n; ++i )
    {
        fprintf(fp," %.16lf\n",array[i]);
    }
    fclose(fp);
    ierr = PetscPrintf(PETSC_COMM_WORLD, "%s closed successfully\n", fileName);CHKERRQ(ierr);

    return ierr;
}

