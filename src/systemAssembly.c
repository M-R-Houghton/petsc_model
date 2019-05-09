#include "systemAssembly.h"

/* Initiates system assembly routine */
PetscErrorCode systemAssembly(Box *box_ptr, Parameters *par_ptr, Mat H, Vec b)
{
    PetscErrorCode ierr;

    /*
     * Calculate sparsity of global matrix
     */

    /*
     * Pre-allocate memory for global matrix
     */

    /*
     * Assemble local contributions and add to global matrix
     */
    ierr = addLocalContributions(box_ptr, par_ptr, H, b);CHKERRQ(ierr);

    /*
     * Assemble matrix (REFERENCE)
     */
    /*
    PetscInt       i,n = 10,col[3];
    PetscScalar    value[3];
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
    ierr = MatSetValues(H,1,&i,2,col,value,INSERT_VALUES);CHKERRQ(ierr); // where &i is an array of i
    */

    ierr = MatAssemblyBegin(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    ierr = MatAssemblyEnd(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

    /* zero entries for elastic medium validation only */
    //ierr = VecZeroEntries(b);CHKERRQ(ierr);
    //ierr = MatZeroEntries(H);CHKERRQ(ierr);

    ierr = MatView(H,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

    ierr = VecView(b,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    return ierr;
}


PetscErrorCode applyElasticMediumToMatrix(Mat H, PetscScalar lambda)
{
    PetscErrorCode ierr;

    ierr = MatShift(H, lambda);CHKERRQ(ierr);

    return ierr;
}


PetscErrorCode applyElasticMediumToRHSVector(Box *box_ptr, Vec B, PetscScalar lambda)
{
    PetscErrorCode 	ierr;
    PetscInt        i,j;
    PetscInt        N = box_ptr->nodeInternalCount;

    for (i = 0; i < box_ptr->nodeCount; i++)
    {
        Node *node = &(box_ptr->masterNodeList[i]);
        if (node->globalID != -1)
        {
            for (j = 0; j < DIMENSION; j++)
            {
                ierr = VecSetValue(B, node->globalID + j*N, lambda * node->xyzAffDisplacement[j], ADD_VALUES);
                CHKERRQ(ierr);
            }
        }
    }

    return ierr;
}


/* Applies a uniform force such that the network behaves suspended in an elastic medium */
PetscErrorCode applyElasticMedium(Box *box_ptr, Mat H, Vec B, PetscScalar lambda)
{
    PetscErrorCode 	ierr;

    ierr = applyElasticMediumToMatrix(H, lambda);

    ierr = applyElasticMediumToRHSVector(box_ptr, B, lambda);

    return ierr;
}


/* Assembles a global vector from the affine displacements of every internal node */
PetscErrorCode assembleAffineDisplacementVector(Box *box_ptr, Vec U_aff)
{
    PetscErrorCode  ierr;
    PetscErrorCode  i,j;
    PetscInt        N = box_ptr->nodeInternalCount;

    for (i = 0; i < box_ptr->nodeCount; i++)
    {
        Node *node = &(box_ptr->masterNodeList[i]);
        if (node->globalID != -1)
        {
            for (j = 0; j < DIMENSION; j++)
            {
                ierr = VecSetValue(U_aff, node->globalID + j*N, node->xyzAffDisplacement[j], INSERT_VALUES);
                CHKERRQ(ierr);
            }
        }
    }

    return ierr;
}


/* Solves a matrix from pre-assembled arrays */
PetscErrorCode solveAssembledMatrix(char const *rowFile, char const *colFile, char const *matFile, 
        char const *rhsFile, char const *solFile, PetscInt n)
{
    PetscErrorCode 	ierr;
    Mat            	H;
    Vec 			B,U;
    KSP            	ksp;          /* linear solver context */
    PC             	pc;           /* preconditioner context */
    PetscInt 		i,its;

    PetscInt 		rowArray[n+1];
    PetscScalar		rhsArray[n];
    PetscScalar		*solArray;

    /* Read in row file to array */
    ierr = readInt(rowFile, rowArray, n+1);CHKERRQ(ierr);

    /* Determine the number of nonzeros */
    PetscInt 		nz = rowArray[n];

    /* Use final value of row ptr array to determine length of col and mat arrays */
    //PetscInt 		colArray[nz];	/* static allocation */
    //PetscScalar 	valArray[nz];	/* static allocation */

    /* dynamic allocation for larger problems */
    PetscInt 		*colArray;
    PetscScalar		*valArray;
    ierr = PetscMalloc(nz*sizeof(PetscInt), &colArray);CHKERRQ(ierr);
    ierr = PetscMalloc(nz*sizeof(PetscScalar),&valArray);CHKERRQ(ierr);

    /* Read in further files to arrays */
    ierr = readInt(colFile, colArray, nz);CHKERRQ(ierr);
    ierr = readDbl(matFile, valArray, nz);CHKERRQ(ierr);
    ierr = readDbl(rhsFile, rhsArray, n);CHKERRQ(ierr);

    /* Vector assembly for existing array */
    ierr = VecCreateSeqWithArray(PETSC_COMM_WORLD,1,n,rhsArray,&B);CHKERRQ(ierr);

    /* Set up solution vector */
    ierr = VecDuplicate(B,&U);CHKERRQ(ierr);
    ierr = VecSet(U,1.0);CHKERRQ(ierr);

    /* Print vector to verify assembly */
    //ierr = VecView(B,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

    /* Matrix assembly for existing CSR arrays */
    //ierr = MatCreateSeqAIJWithArrays(PETSC_COMM_WORLD,n,n,rowArray,colArray,valArray,&H);CHKERRQ(ierr);

    /* Row at a time manual matrix assembly */
    ///*
    ierr = MatCreate(PETSC_COMM_WORLD,&H);CHKERRQ(ierr);
    ierr = MatSetSizes(H,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);

    ierr = MatSetFromOptions(H);CHKERRQ(ierr);
    ierr = MatSetUp(H);CHKERRQ(ierr);

    PetscInt 	tmpCol[n];
    PetscScalar tmpMat[n];

    for (i = 0; i < n; i++)
    {
        PetscInt j,count = 0;

        for (j = rowArray[i]; j < rowArray[i+1]; j++)
        {
            tmpCol[count] = colArray[j];
            tmpMat[count] = valArray[j];
            count += 1;
        }
        ierr = MatSetValues(H,1,&i,rowArray[i+1]-rowArray[i],tmpCol,tmpMat,INSERT_VALUES);CHKERRQ(ierr);		// produces 1st row where 1st row should be
        //ierr = MatSetValues(H,2,&i,3,colArray,valArray,INSERT_VALUES);CHKERRQ(ierr);		// produces 2nd row where 1st row should be
        //ierr = MatSetValues(H,3,&i,3,colArray,valArray,INSERT_VALUES);CHKERRQ(ierr);		// produces 3rd row where 1st row should be
    }

    ierr = MatAssemblyBegin(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    ierr = MatAssemblyEnd(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    //*//* END manual matrix assembly */

    /* Free any dynamic allocation */
    ierr = PetscFree(colArray);CHKERRQ(ierr);
    ierr = PetscFree(valArray);CHKERRQ(ierr);

    /* Print matrix to verify assembly */
    //ierr = MatView(H,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    //ierr = MatView(H,PETSC_VIEWER_DRAW_WORLD);CHKERRQ(ierr);

    /* Create solver context */
    ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);

    /* Choose matrix as it's own preconditioning matrix */
    ierr = KSPSetOperators(ksp,H,H MATSTRUCT);CHKERRQ(ierr);

    ierr = KSPSetType(ksp, KSPPREONLY);

    /* Set up default options */
    ierr = KSPGetPC(ksp,&pc);CHKERRQ(ierr);
    ierr = PCSetType(pc,PCLU);CHKERRQ(ierr);
    ierr = KSPSetTolerances(ksp,1.e-5,PETSC_DEFAULT,PETSC_DEFAULT,PETSC_DEFAULT);CHKERRQ(ierr);

    /* Set runtime options */
    ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr);

    /* Solve linear system */
    ierr = KSPSolve(ksp,B,U);CHKERRQ(ierr);

    /* View solver info */
    //ierr = KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

    /* Get iteration count */
    ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"Iterations %D\n",its);CHKERRQ(ierr);

    //ierr = VecView(U,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

    /* Check out array */
    ierr = VecGetArray(U, &solArray);CHKERRQ(ierr);
    /* Write out array */
    ierr = writeDbl(solFile, solArray, n);CHKERRQ(ierr);
    /* Check array back in */
    ierr = VecRestoreArray(U, &solArray);CHKERRQ(ierr);

    /* Clean up */
    ierr = VecDestroy(&B);CHKERRQ(ierr);
    ierr = VecDestroy(&U);CHKERRQ(ierr);
    ierr = MatDestroy(&H);CHKERRQ(ierr);
    ierr = KSPDestroy(&ksp);CHKERRQ(ierr);

    return ierr;
}


/* Reads in a file of integers to an array */
PetscErrorCode readInt(char const *fileName, PetscInt *array, PetscInt n)
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
PetscErrorCode readDbl(char const *fileName, PetscScalar *array, PetscInt n)
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
PetscErrorCode writeDbl(char const *fileName, PetscScalar *array, PetscInt n) 
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

