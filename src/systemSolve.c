#include "systemSolve.h"

/* Initiates system solve routine */
PetscErrorCode systemSolve(Mat globalMat_H, Vec globalVec_B, Vec globalVec_U)
{
	PetscErrorCode 	ierr;
	KSP            	ksp;          /* linear solver context */
	PC             	pc;           /* preconditioner context */
	PetscInt 		its;
	PetscReal 		norm; 		  /* norm of solution error */

    ierr = MatGetDiagonal(globalMat_H,globalVec_U);CHKERRQ(ierr);
	//ierr = VecView(globalVec_U,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	/*
       Create linear solver context
    */
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Creating solver context...\n");CHKERRQ(ierr);
	ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);
	
	/*
       Set operators. Here the matrix that defines the linear system
       also serves as the preconditioning matrix.
    */
	ierr = KSPSetOperators(ksp,globalMat_H,globalMat_H MATSTRUCT);CHKERRQ(ierr);


	/*
	  Set linear solver defaults for this problem (optional).
	  - By extracting the KSP and PC contexts from the KSP context,
	    we can then directly call any KSP and PC routines to set
	    various options.
	  - The following four statements are optional; all of these
	    parameters could alternatively be specified at runtime via
	    KSPSetFromOptions();
	*/
	ierr = KSPGetPC(ksp,&pc);CHKERRQ(ierr);
	ierr = PCSetType(pc,PCJACOBI);CHKERRQ(ierr);
	ierr = KSPSetTolerances(ksp,1.e-5,PETSC_DEFAULT,PETSC_DEFAULT,PETSC_DEFAULT);CHKERRQ(ierr);


	/*
      Set runtime options, e.g.,
          -ksp_type <type> -pc_type <type> -ksp_monitor -ksp_rtol <rtol>
      These options will override those specified above as long as
      KSPSetFromOptions() is called _after_ any other customization
      routines.
    */
    ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr);

    /*
	  Solve linear system
	*/
	ierr = KSPSolve(ksp,globalVec_B,globalVec_U);CHKERRQ(ierr);

	//ierr = VecView(globalVec_U,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/*
	  View solver info; we could instead use the option -ksp_view to
	  print this info to the screen at the conclusion of KSPSolve().
	*/
	ierr = KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/* Get iteration count */
	ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Iterations %d\n",its);CHKERRQ(ierr);

    /* Manual residual norm calculation */
    /*
    PetscReal   myNorm;
    Vec         v,w,V;
    ierr = KSPSetNormType(ksp, KSP_NORM_UNPRECONDITIONED);
    ierr = KSPBuildResidual(ksp,v,w,&V);CHKERRQ(ierr);
    ierr = VecNorm(V, NORM_INFINITY, &myNorm);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"Actual norm = %g\n",(double)myNorm);CHKERRQ(ierr);
    VecDestroy(&v);
    VecDestroy(&w);
    */

	/* Get (relative?) residual norm */
	KSPGetResidualNorm(ksp,&norm);
	PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Residual norm = %g\n", (double)norm);CHKERRQ(ierr);

	ierr = KSPDestroy(&ksp);CHKERRQ(ierr);

	return ierr;
}


PetscErrorCode printLargeVecValues(Vec globalVec_F)
{
    PetscErrorCode  ierr = 0;
    PetscInt        i,nlocal;
    PetscInt        c3e7=0,c2e7=0,c1e7=0;
    PetscInt        ce8=0,ce9=0,ce10=0,ce11=0,ce12=0;
    PetscInt        cOther=0;
    PetscInt        nodeSum;    
    PetscScalar     const *array;
    Vec             absVec_F;

    ierr = VecDuplicate(globalVec_F, &absVec_F);CHKERRQ(ierr);
    ierr = VecCopy(globalVec_F, absVec_F);CHKERRQ(ierr);
    ierr = VecAbs(absVec_F);CHKERRQ(ierr);

    ierr = VecGetLocalSize(absVec_F,&nlocal);
    ierr = VecGetArrayRead(absVec_F,&array);CHKERRQ(ierr);
    for (i = 0; i < nlocal; i++)
    {
        if (array[i] >= 3e-7)
        {
            //ierr = PetscPrintf(PETSC_COMM_WORLD,"array[%d] = %g\n", i, array[i]);CHKERRQ(ierr);
            c3e7 += 1;
        }
        else if (2e-7 <= array[i] && array[i] < 3e-7)
        {              
            c2e7 += 1; 
        }              
        else if (1e-7 <= array[i] && array[i] < 2e-7)
        {              
            c1e7 += 1; 
        }              
        else if (1e-8 <= array[i] && array[i] < 1e-7)
        {              
            ce8 += 1; 
        }              
        else if (1e-9 <= array[i] && array[i] < 1e-8)
        {
            ce9 += 1;
        }
        else if (1e-10 <= array[i] && array[i] < 1e-9)
        {
            ce10 += 1;
        }
        else if (1e-11 <= array[i] && array[i] < 1e-10)
        {
            ce11 += 1;
        }
        else if (1e-12 <= array[i] && array[i] < 1e-11)
        {
            ce12 += 1;
        }
        else if (array[i] < 1e-12)
        {
            cOther += 1;
        }
    }
    ierr = VecRestoreArrayRead(absVec_F,&array);CHKERRQ(ierr);

    //ierr = PetscPrintf(PETSC_COMM_WORLD,"Counts are:\n");CHKERRQ(ierr);
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"%d\n%d\n%d\n",c3e7,c2e7,c1e7);CHKERRQ(ierr);
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"%d\n%d\n%d\n",ce8,ce9,ce10);CHKERRQ(ierr);
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"%d\n%d\n%d\n",ce11,ce12,cOther);CHKERRQ(ierr);

    nodeSum = c3e7+c2e7+c1e7+ce8+ce9+ce10+ce11+ce12+cOther; 
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"Checking consistency:\n");CHKERRQ(ierr);
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"%d = %d\n", nodeSum, nlocal);CHKERRQ(ierr);

    VecDestroy(&absVec_F);

    return ierr;
}


/* Initiates time-stepping solve routine */
PetscErrorCode systemTimeStepSolve(Mat globalMat_H, Vec globalVec_B, Vec globalVec_U, 
                                    PetscScalar alpha, PetscScalar normTolF, PetscInt maxSteps)
{
    /* 
     * solves the system 
     *  F = - (HU - B) 
     * via time stepping
     */

    PetscErrorCode  ierr = 0;
    PetscInt        steps = 0;
    PetscInt        printSteps = 10000;
    PetscInt        maxInd, minInd;
    PetscReal       maxVal, minVal;
    PetscReal       initialNormF,prevNormF,normF = 0;
    Vec             globalVec_F,globalVec_pU,globalVec_ppU;
    PetscViewer     viewer;

    /* set up vecs for storing prev U and prev prev U */
    ierr = VecDuplicate(globalVec_U, &globalVec_pU);CHKERRQ(ierr);      /* duplicate vec size and type */
    ierr = VecDuplicate(globalVec_U, &globalVec_ppU);CHKERRQ(ierr);
    ierr = VecCopy(globalVec_U, globalVec_pU);CHKERRQ(ierr);            /* copy contents of vec to another */
    ierr = VecCopy(globalVec_U, globalVec_ppU);CHKERRQ(ierr);

    /* set up F */
    ierr = VecDuplicate(globalVec_U, &globalVec_F);CHKERRQ(ierr);
    ierr = VecSet(globalVec_F, 0.0);CHKERRQ(ierr);
    
    ierr = PetscPrintf(PETSC_COMM_WORLD,"alpha = %g\n",alpha);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"normTolF = %g\n",normTolF);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"maxSteps = %d\n",maxSteps);CHKERRQ(ierr);
   
    /* set B' = -B */
    ierr = VecScale(globalVec_B, -1.0);CHKERRQ(ierr);
    while (steps < maxSteps)
    {
        /*
         * set F' = HU + B'
         * set F  = -F'
         * update U -> U + alpha*F 
         */
        ierr = MatMultAdd(globalMat_H, globalVec_U, globalVec_B, globalVec_F);CHKERRQ(ierr); 
        ierr = VecScale(globalVec_F, -1.0);CHKERRQ(ierr);
        ierr = VecAXPY(globalVec_U, alpha, globalVec_F);CHKERRQ(ierr);

        /* calculate the norm */
        ierr = VecNorm(globalVec_F, NORM_INFINITY, &normF);CHKERRQ(ierr);

        /* previous normF is set initially the same as normF */
        if (steps == 0) 
        {
            initialNormF = normF;           /* initialNormF no longer needed - used only for printing */
            prevNormF = normF;
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Initial Res. Norm = %g\n",initialNormF);CHKERRQ(ierr);
        }
        else if (steps%printSteps == 0) 
        {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Res. Norm at %d = %g\n",steps,normF);CHKERRQ(ierr);
        }

        /* check for termination */
        if (normF < normTolF)               /* stop when norm is below tolerance */ 
        {
            /* if no divergence then analyse current U (after termination of this function) */
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Final Res. Norm = %g\n",normF);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"After %d Steps.\n",steps);CHKERRQ(ierr);
            ierr = VecAbs(globalVec_F);CHKERRQ(ierr);
            ierr = VecMax(globalVec_F,&maxInd,&maxVal);CHKERRQ(ierr);
            ierr = VecMin(globalVec_F,&minInd,&minVal);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"(step %d) Max val = %g, at index = %d\n",steps,(double)maxVal,maxInd);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"(step %d) Min val = %g, at index = %d\n",steps,(double)minVal,minInd);CHKERRQ(ierr);
            break;
        }
        else if (isinf(normF) || isnan(normF))
        {
            /* if divergence then analyse U from 2 steps ago (after termination of this function) */
            ierr = PetscPrintf(PETSC_COMM_WORLD,"[ERROR] Divergence at step %d. Unstable Res. Norm = %g\n",steps,normF);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Prev. (step %d) Res. Norm = %g\n",steps-1,prevNormF);CHKERRQ(ierr);
            break;
        }
        else if (normF > 10*prevNormF)         /* check for divergence at every step */
        {
            /* if divergence then analyse U from 2 steps ago (after termination of this function) */
            ierr = PetscPrintf(PETSC_COMM_WORLD,"[ERROR] Divergence at step %d. Unstable Res. Norm = %g\n",steps,normF);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Prev. (step %d) Res. Norm = %g\n",steps-1,prevNormF);CHKERRQ(ierr);
            ierr = VecCopy(globalVec_ppU, globalVec_U);CHKERRQ(ierr);

            ierr = VecAbs(globalVec_F);CHKERRQ(ierr);
            ierr = VecMax(globalVec_F,&maxInd,&maxVal);CHKERRQ(ierr);
            ierr = VecMin(globalVec_F,&minInd,&minVal);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"(step %d) Max val = %g, at index = %d\n",steps,(double)maxVal,maxInd);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"(step %d) Min val = %g, at index = %d\n",steps,(double)minVal,minInd);CHKERRQ(ierr);

            ierr = printLargeVecValues(globalVec_F);CHKERRQ(ierr);

            /* additional FINAL write that is just for testing */
            //ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,"vector.dat",FILE_MODE_WRITE,&viewer);CHKERRQ(ierr);
            //ierr = VecView(globalVec_pU,viewer);
            //ierr = PetscViewerDestroy(&viewer);
            break;
        }
        else                                /* if no termination then write U from 2 steps ago */
        {
            /* node displacement oscillation check */
            //PetscInt        index=4177;
            //PetscScalar     const *array;

            //ierr = VecGetArrayRead(globalVec_U,&array);CHKERRQ(ierr);
            //ierr = PetscPrintf(PETSC_COMM_WORLD,"array[%d] = %g\n",index,array[index]);CHKERRQ(ierr);
            //index = 12865;      // i.e. 4177 + 8688*1
            //ierr = PetscPrintf(PETSC_COMM_WORLD,"array[%d] = %g\n",index,array[index]);CHKERRQ(ierr);
            //index = 21553;      // i.e. 4177 + 8688*2
            //ierr = PetscPrintf(PETSC_COMM_WORLD,"array[%d] = %g\n",index,array[index]);CHKERRQ(ierr);
            //ierr = VecRestoreArrayRead(globalVec_U,&array);CHKERRQ(ierr);

            ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,"vector.dat",FILE_MODE_WRITE,&viewer);CHKERRQ(ierr);
            ierr = VecView(globalVec_ppU,viewer);
            ierr = PetscViewerDestroy(&viewer);
        }

        /* update ppU after 2 steps and pU after 1 step */
        if (steps > 1)
        {
            ierr = VecCopy(globalVec_pU, globalVec_ppU);CHKERRQ(ierr);
            ierr = VecCopy(globalVec_U, globalVec_pU);CHKERRQ(ierr);
        }
        else if (steps > 0)
        {
            ierr = VecCopy(globalVec_U, globalVec_pU);CHKERRQ(ierr);
        }

        /* update prevNormF after every step and increment */
        prevNormF = normF;
        steps += 1;
    }

    /* use for debugging small cases */
    //ierr = VecView(globalVec_U, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    
    /* clean up */
    VecDestroy(&globalVec_F);
    VecDestroy(&globalVec_pU);
    VecDestroy(&globalVec_ppU);

    return ierr;
}
