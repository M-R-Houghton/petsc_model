#include "systemAssembly.h"

/* Initiates system assembly routine */
PetscErrorCode systemAssembly(const Box *box_ptr, const Parameters *par_ptr, Mat H, Vec b)
{
    PetscErrorCode  ierr   = 0;
    PetscBool       useEM  = PETSC_FALSE;    /* set default values */
    PetscScalar     lambda = 1e-5;       

    /* set up options for elastic medium */
    ierr = PetscOptionsGetBool(NULL,NULL,"-use_em",&useEM,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetReal(NULL,NULL,"-k",&lambda,NULL);CHKERRQ(ierr);

    /* Calculate sparsity of global matrix */
    // TODO: implement this

    /* Pre-allocate memory for global matrix */
    // TODO: implement this

    /* Assemble local contributions and add to global matrix */
    ierr = addLocalContributions(box_ptr, par_ptr, H, b);CHKERRQ(ierr);

    /* Embed network in elastic medium if specified by user */
    if (useEM)
    {
        ierr = applyElasticMedium(box_ptr, H, b, lambda);CHKERRQ(ierr);
    }

    /* Assemble after all VecSets and MatSets */
    ierr = VecAssemblyBegin(b);CHKERRQ(ierr);
    ierr = VecAssemblyEnd(b);CHKERRQ(ierr);
    ierr = MatAssemblyBegin(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    ierr = MatAssemblyEnd(H,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

    ierr = PetscPrintf(PETSC_COMM_WORLD, "Printing vec B sample here...\n");CHKERRQ(ierr);
    //ierr = printVecValuesSample(b);CHKERRQ(ierr);

    /* Zero entries for elastic medium validation only */
    //ierr = VecZeroEntries(b);CHKERRQ(ierr);
    //ierr = MatZeroEntries(H);CHKERRQ(ierr);

    /* View global matrix and rhs vector if problem is small enough */
    PetscInt solSize;
    ierr = VecGetSize(b, &solSize);CHKERRQ(ierr);
    if (solSize < 50)
    {
        ierr = MatView(H,PETSC_VIEWER_DRAW_WORLD);CHKERRQ(ierr);
        ierr = VecView(b,PETSC_VIEWER_DRAW_WORLD);CHKERRQ(ierr);
    }

    return ierr;
}


PetscErrorCode printVecValuesSample(Vec vecToPrint)
{
    PetscErrorCode ierr;
    PetscInt i,nlocal;
    PetscInt sample=10;
    PetscScalar const *array;

    ierr = VecGetLocalSize(vecToPrint,&nlocal);
    ierr = VecGetArrayRead(vecToPrint,&array);CHKERRQ(ierr);
    if (nlocal >= sample)
    {
        for (i = 0; i < sample; i++)
        {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"array[%d] = %.16g\n", i, array[i]);CHKERRQ(ierr);
        }
    }
    ierr = VecRestoreArrayRead(vecToPrint,&array);CHKERRQ(ierr);
    
    return ierr;
}


PetscErrorCode applyEMToDecoupledMatrix(Mat H, const PetscScalar lambda, const PetscInt N)
{
    PetscErrorCode  ierr = 0;
    PetscInt        nID,i;

    // TODO: Will not work before assembly. For reference only - remove.
    //ierr = MatShift(H, lambda);CHKERRQ(ierr);

    /* update diagonals by looping over each internal ID */
    for (nID = 0; nID < N; nID++)
    {
        /* update diagonal value at each coordinate */
	    for (i = 0; i < DIMENSION; i++)
	    {
            /* for decoupled systems apply uniform force to all diagonals */
	        ierr = MatSetValue(H, nID + i*N, nID + i*N, lambda, ADD_VALUES);CHKERRQ(ierr);
	    }
    }
    return ierr;
}


PetscErrorCode applyEMToCoupledMatrix(Mat H, const PetscScalar lambda, 
                                        const PetscInt coupleCount, const Couple *coupleList)
{
    PetscErrorCode  ierr = 0;
    PetscInt        cID,i; 

    // TODO: Here we either need to loop over all nodes in a couple, adding for each one.
    // ...or we need to add lambda as a factor of the number of nodes in the couple.

    /* update diagonals by looping over each couple */
    for (cID = 0; cID < coupleCount; cID++)
    {
        PetscInt numNodes = coupleList[cID].nodesInCouple;
        /* update diagonal value at each coordinate */
	    for (i = 0; i < DIMENSION; i++)
	    {
	        ierr = MatSetValue(H, cID + i*coupleCount, cID + i*coupleCount, numNodes*lambda, ADD_VALUES);CHKERRQ(ierr);
	    }
    }

    return ierr;
}


PetscErrorCode applyElasticMediumToMatrix(Mat H, const PetscScalar lambda, const PetscInt internalCount, 
                                            const PetscInt coupleCount, const Couple *coupleList)
{
    PetscErrorCode ierr;

    if (coupleCount == 0)
    {
        ierr = applyEMToDecoupledMatrix(H, lambda, internalCount);CHKERRQ(ierr);
    }
    else
    {
        ierr = applyEMToCoupledMatrix(H, lambda, coupleCount, coupleList);CHKERRQ(ierr);
    }

    return ierr;
}


PetscErrorCode applyEMToDecoupledRHSVector(const Box *box_ptr, Vec B, const PetscScalar lambda)
{
    PetscErrorCode  ierr = 0;
    PetscInt        N = box_ptr->nodeInternalCount;
    PetscInt        i,j;

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


PetscErrorCode applyEMToCoupledRHSVector(const Box *box_ptr, Vec B, const PetscScalar lambda)
{
    PetscErrorCode  ierr = 0;
    PetscInt        N = box_ptr->nodeInternalCount;
    PetscInt        i,j,k;

    /* Need to loop over coupleCount so we can access each nodes contribution from a couple */
    for (i = 0; i < box_ptr->coupleCount; i++)
    {
        Couple *couple = &(box_ptr->masterCoupleList[i]);
        assert(couple->coupleID == i);
        assert(couple->nodesInCouple > 0);

        for (j = 0; j < couple->nodesInCouple; j++)
        {
            /* take each node of the couple */
            Node *node_j = &(box_ptr->masterNodeList[couple->nodeID[j]]);
            assert(node_j->globalID == couple->coupleID);

            /* for x,y,z components shift node a factor of the affine displacement vector */
            for (k = 0; k < DIMENSION; k++)
            {
                ierr = VecSetValue(B, node_j->globalID + k*N, lambda * node_j->xyzAffDisplacement[k], ADD_VALUES);
            }
        }
    }
    return ierr;
}


PetscErrorCode applyElasticMediumToRHSVector(const Box *box_ptr, Vec B, const PetscScalar lambda)
{
    PetscErrorCode 	ierr;

    if (box_ptr->coupleCount == 0)
    {
        /* decoupled networks are handled by looping over all internal nodes */
        ierr = applyEMToDecoupledRHSVector(box_ptr, B, lambda);CHKERRQ(ierr);
    }
    else
    {
        ierr = applyEMToCoupledRHSVector(box_ptr, B, lambda);CHKERRQ(ierr);
    }
    return ierr;
}


/* Applies a uniform force such that the network behaves suspended in an elastic medium */
PetscErrorCode applyElasticMedium(const Box *box_ptr, Mat H, Vec B, const PetscScalar lambda)
{
    PetscErrorCode 	ierr;

    /* Networks with internals not associated with couples need handling separately */
    if (box_ptr->coupleCount != 0 && box_ptr->nodeInternalCount != box_ptr->coupleCount)
    {
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[ERROR] Not able to handle this type of network yet.\n");CHKERRQ(ierr);
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[ERROR] Please run w/o EM or use another network.\n");CHKERRQ(ierr);
        exit(1);
    }

    ierr = applyElasticMediumToMatrix( H, lambda, box_ptr->nodeInternalCount, 
                                        box_ptr->coupleCount, box_ptr->masterCoupleList );
    ierr = applyElasticMediumToRHSVector(box_ptr, B, lambda);

    return ierr;
}


/* Assembles a global vector from the affine displacements of every internal node */
PetscErrorCode assembleAffineDisplacementVector(const Box *box_ptr, Vec U_aff)
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

// TODO: Tidy up or remove completely
/* Solves a matrix from pre-assembled arrays */
PetscErrorCode solveAssembledMatrix(const char *rowFile, const char *colFile, const char *matFile, 
        const char *rhsFile, const char *solFile, PetscInt n)
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
PetscErrorCode readInt(const char *fileName, PetscInt *array, PetscInt n)
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
PetscErrorCode readDbl(const char *fileName, PetscScalar *array, PetscInt n)
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
PetscErrorCode writeDbl(const char *fileName, PetscScalar *array, PetscInt n) 
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

