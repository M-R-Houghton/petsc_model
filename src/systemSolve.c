#include "systemSolve.h"

/* Initiates system solve routine */
PetscErrorCode systemSolve(Mat globalMat_H, Vec globalVec_B, Vec globalVec_U)
{
	PetscErrorCode 	ierr;
	KSP            	ksp;          /* linear solver context */
	PC             	pc;           /* preconditioner context */

	/*
       Create linear solver context
    */
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Creating solver context...\n");CHKERRQ(ierr);
	ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);
	
	/*
       Set operators. Here the matrix that defines the linear system
       also serves as the preconditioning matrix.
    */
	ierr = KSPSetOperators(ksp,globalMat_H,globalMat_H);CHKERRQ(ierr);


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

	ierr = VecView(globalVec_U,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/*
	  View solver info; we could instead use the option -ksp_view to
	  print this info to the screen at the conclusion of KSPSolve().
	*/
	ierr = KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	ierr = KSPDestroy(&ksp);CHKERRQ(ierr);

	return ierr;
}

