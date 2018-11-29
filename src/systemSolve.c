#include "systemSolve.h"

/* Initiates system solve routine */
PetscErrorCode systemSolve(Mat globalMat_H, Vec globalVec_B, Vec globalVec_U)
{
	PetscErrorCode 	ierr;
	KSP            	ksp;          /* linear solver context */
	PC             	pc;           /* preconditioner context */
	PetscInt 		its;
	PetscReal 		norm; 		  /* norm of solution error */

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

	/* Get (relative?) residual norm */
	KSPGetResidualNorm(ksp,&norm);
	PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Residual norm = %e\n", (double)norm);CHKERRQ(ierr);

	ierr = KSPDestroy(&ksp);CHKERRQ(ierr);

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
    PetscInt        printSteps = 1000;
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
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Max val = %g, at index = %d\n",(double)maxVal,maxInd);CHKERRQ(ierr);
            break;
        }
        else if (normF > prevNormF)         /* check for divergence at every step */
        {
            /* if divergence then analyse U from 2 steps ago (after termination of this function) */
            ierr = PetscPrintf(PETSC_COMM_WORLD,"[ERROR] Divergence. Unstable Res. Norm = %g\n", normF);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Final Res. Norm = %g\n",prevNormF);CHKERRQ(ierr);
            ierr = VecCopy(globalVec_ppU, globalVec_U);CHKERRQ(ierr);
            ierr = VecAbs(globalVec_F);CHKERRQ(ierr);
            ierr = VecMax(globalVec_F,&maxInd,&maxVal);CHKERRQ(ierr);
            ierr = VecMin(globalVec_F,&minInd,&minVal);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Max val = %g, at index = %d\n",(double)maxVal,maxInd);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Min val = %g, at index = %d\n",(double)minVal,minInd);CHKERRQ(ierr);
            break;
        }
        else                                /* if no termination then write U from 2 steps ago */
        {
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
