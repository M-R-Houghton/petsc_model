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
    PetscInt        printSteps = 10000;
    PetscReal       initialNormF,prevNormF,normF = 0;
    Vec             globalVec_F;

    /* set up F */
    VecDuplicate(globalVec_U, &globalVec_F);
    VecSet(globalVec_F, 0.0);
    
    PetscPrintf(PETSC_COMM_WORLD,"alpha = %g\n",alpha);
    PetscPrintf(PETSC_COMM_WORLD,"normTolF = %g\n",normTolF);
    PetscPrintf(PETSC_COMM_WORLD,"maxSteps = %d\n",maxSteps);
   
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
        if (steps%printSteps == 0) PetscPrintf(PETSC_COMM_WORLD,"Res. Norm at %d = %g\n",steps,normF);
        if (steps == 0) 
        {
            initialNormF = normF;
            prevNormF = normF;
        }
        
        if (normF < normTolF)   /* stop when norm is below tolerance */ 
        {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Final Res. Norm = %g\n",normF);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"After %d Steps.\n",steps);CHKERRQ(ierr);
            break;
        }
        else if (normF > 10*initialNormF)   /* or if norm grows beyond starting value */
        {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"[ERROR] Divergence. Try alpha<%g",alpha);CHKERRQ(ierr);
            break;
        }

        if (steps%printSteps == 0)
        {
            if (normF > prevNormF)  /* also check for divergence periodically */
            {
                ierr = PetscPrintf(PETSC_COMM_WORLD,"[ERROR] Divergence.");CHKERRQ(ierr);
                break;
            }
            prevNormF = normF;
        }
        steps += 1;
    }

    /* use for debugging small cases */
    //ierr = VecView(globalVec_U, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    
    /* clean up */
    VecDestroy(&globalVec_F);

    return ierr;
}
