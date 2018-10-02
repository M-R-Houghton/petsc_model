
static char help[] = "The first draft of the PETSc model.\n\n";

#include "networkWrite.h"
#include "integrationTests.h"

int main(int argc, char **args)
{
	Box  		   *box_ptr;
	Parameters 	   *par_ptr;
	Vec            x, b, u;      /* approx solution, RHS, exact solution */
  	Mat            A;            /* linear system matrix */
  	KSP            ksp;          /* linear solver context */
	//PC             pc;           /* preconditioner context */
  	//PetscReal      norm;         /* norm of solution error */
	PetscErrorCode ierr;
	PetscInt       n,N;
	//PetscInt 	   its;
	PetscMPIInt    size;
	PetscScalar    one = 1.0;
	PetscBool      nonzeroguess = PETSC_FALSE;
	//PetscBool 	   changepcside = PETSC_FALSE;
#if defined(PETSC_USE_LOG)
	PetscLogStage stages[6];
#endif

	if (argc < 2)
	{
		printf("[ERROR] Incorrect number of arguments provided.\n");
		exit(EXIT_FAILURE);
	}

	/* set options file */
	const char optFile[] = "modelOptions.dat";

	/* set parameter file */
	char *parFile = args[1];

	printf("[STATUS] Initialising...\n");
	ierr = PetscInitialize(&argc,&args,optFile,help);if (ierr) return ierr;
	ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);CHKERRQ(ierr);
  	if (size != 1) SETERRQ(PETSC_COMM_WORLD,1,"This is a uniprocessor example only!");
	ierr = PetscOptionsGetInt(NULL,NULL,"-n",&n,NULL);CHKERRQ(ierr);
	ierr = PetscOptionsGetBool(NULL,NULL,"-nonzero_guess",&nonzeroguess,NULL);CHKERRQ(ierr);

	/* perform all unit tests */
	ierr = runIntegrationTests();CHKERRQ(ierr);

	/* Register stages for separate profiling */
	ierr = PetscLogStageRegister("Network Read-in",  &stages[0]);CHKERRQ(ierr);
	ierr = PetscLogStageRegister("System Assembly",  &stages[1]);CHKERRQ(ierr);
	ierr = PetscLogStageRegister("System Solve",  	 &stages[2]);CHKERRQ(ierr);
	ierr = PetscLogStageRegister("Network Update", 	 &stages[3]);CHKERRQ(ierr);
	ierr = PetscLogStageRegister("Network Analysis", &stages[4]);CHKERRQ(ierr);
	ierr = PetscLogStageRegister("Network Write-out",&stages[5]);CHKERRQ(ierr);

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading parameter file...\n");CHKERRQ(ierr);
	ierr = parameterRead(parFile, &par_ptr);CHKERRQ(ierr);

	/* read in network data file */
	ierr = PetscLogStagePush(stages[0]);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading from file...\n");CHKERRQ(ierr);
	ierr = networkRead(par_ptr->inputNetwork, &box_ptr, par_ptr->gamma);CHKERRQ(ierr);
	N 	 = box_ptr->nodeInternalCount;
	n 	 = N * DIMENSION;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Problem size is (%d x %d)\n", n, n);CHKERRQ(ierr);
	ierr = PetscLogStagePop();CHKERRQ(ierr);

	/* simple check to make sure macro is correctly set */
	if (box_ptr->xyzDimension[2] == 0.0) assert(DIMENSION == 2);
	if (box_ptr->xyzDimension[2] != 0.0) assert(DIMENSION == 3);

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            Compute the matrix and right-hand-side vector that define
         	the linear system, Ax = b.
     	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* allocate memory for global rhs and solution vectors and set them up */
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting up vectors...\n");CHKERRQ(ierr);
	ierr = VecCreate(PETSC_COMM_WORLD,&x);CHKERRQ(ierr);
	//ierr = PetscObjectSetName((PetscObject) x, "Solution");CHKERRQ(ierr);
	ierr = VecSetSizes(x,PETSC_DECIDE,n);CHKERRQ(ierr);
	ierr = VecSetFromOptions(x);CHKERRQ(ierr);
	ierr = VecDuplicate(x,&b);CHKERRQ(ierr);
	ierr = VecDuplicate(x,&u);CHKERRQ(ierr);

	//ierr = VecView(x,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	//ierr = VecView(b,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	//ierr = VecView(u,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/* allocate memory for global matrix and set it up */
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting up matrix...\n");CHKERRQ(ierr);
	ierr = MatCreate(PETSC_COMM_WORLD,&A);CHKERRQ(ierr);
	ierr = MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);
	ierr = MatSetFromOptions(A);CHKERRQ(ierr);
	ierr = MatSetUp(A);CHKERRQ(ierr);

	/* assemble sparse structure and assemble linear system */
	ierr = PetscLogStagePush(stages[1]);CHKERRQ(ierr);
	ierr = systemAssembly(box_ptr,par_ptr,A,x);CHKERRQ(ierr);
	ierr = PetscLogStagePop();CHKERRQ(ierr);

	/*
       Set exact solution; then compute right-hand-side vector.
    */
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting exact solution...\n");CHKERRQ(ierr);
    ierr = VecSet(u,one);CHKERRQ(ierr);
    ierr = MatMult(A,u,b);CHKERRQ(ierr);

    //ierr = VecView(x,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    //ierr = VecView(b,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            Create the linear solver and set various options
     	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    /*
       Create linear solver context
    */
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Creating solver context...\n");CHKERRQ(ierr);
	//ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);
	
	/*
       Set operators. Here the matrix that defines the linear system
       also serves as the preconditioning matrix.
    */
	//ierr = KSPSetOperators(ksp,A,A);CHKERRQ(ierr);

	/*
	  Set linear solver defaults for this problem (optional).
	  - By extracting the KSP and PC contexts from the KSP context,
	    we can then directly call any KSP and PC routines to set
	    various options.
	  - The following four statements are optional; all of these
	    parameters could alternatively be specified at runtime via
	    KSPSetFromOptions();
	*/
	//*
	//ierr = KSPGetPC(ksp,&pc);CHKERRQ(ierr);
	//ierr = PCSetType(pc,PCJACOBI);CHKERRQ(ierr);
	//ierr = KSPSetTolerances(ksp,1.e-5,PETSC_DEFAULT,PETSC_DEFAULT,PETSC_DEFAULT);CHKERRQ(ierr);
	//*/

	/*
      Set runtime options, e.g.,
          -ksp_type <type> -pc_type <type> -ksp_monitor -ksp_rtol <rtol>
      These options will override those specified above as long as
      KSPSetFromOptions() is called _after_ any other customization
      routines.
    */
    //ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr);

    /*
    if (nonzeroguess) 
    {
		PetscScalar p = .5;
		ierr = VecSet(x,p);CHKERRQ(ierr);
		ierr = KSPSetInitialGuessNonzero(ksp,PETSC_TRUE);CHKERRQ(ierr);
	}
	*/

	/* solve linear system */
	ierr = PetscLogStagePush(stages[2]);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Solving system...\n");CHKERRQ(ierr);
    ierr = systemSolve(A,x,b);CHKERRQ(ierr);
    ierr = PetscLogStagePop();CHKERRQ(ierr);

    ierr = PetscLogStagePush(stages[3]);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Updating network...\n");CHKERRQ(ierr);
    ierr = networkUpdate(box_ptr,b);CHKERRQ(ierr);
    ierr = PetscLogStagePop();CHKERRQ(ierr);


    /* assemble tripod matrix */
    ///*
    char rowFile[100] = "data/row/row.f3tTripod1";
    char colFile[100] = "data/col/col.f3tTripod1";
    char matFile[100] = "data/mat/mat.f3tTripod1";
    char rhsFile[100] = "data/rhs/rhs.f3tTripod1";
    char solFile[100] = "data/sol/sol.f3tTripod1";
    ierr = solveAssembledMatrix(rowFile,colFile,matFile,rhsFile,solFile,3);CHKERRQ(ierr);
    //*/

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            Solve the linear system
     	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    /*
	  Solve linear system
	*/
	//ierr = KSPSolve(ksp,x,b);CHKERRQ(ierr);

	/*
	  View solver info; we could instead use the option -ksp_view to
	  print this info to the screen at the conclusion of KSPSolve().
	*/
	//ierr = KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	//ierr = VecView(x,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	//ierr = VecView(b,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        	Check solution and clean up
     	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/*
      Check the error
    */
    /*
    ierr = VecAXPY(x,-1.0,u);CHKERRQ(ierr);
    ierr = VecNorm(x,NORM_2,&norm);CHKERRQ(ierr);
    ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm of error %g, Iterations %D\n",(double)norm,its);CHKERRQ(ierr);
	*/

    /* make predictions based on solution */
    ierr = PetscLogStagePush(stages[4]);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Analysing network...\n");CHKERRQ(ierr);
    ierr = networkAnalysis();CHKERRQ(ierr);
    ierr = PetscLogStagePop();CHKERRQ(ierr);

    /* write out new network data file */
    ierr = PetscLogStagePush(stages[5]);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Writing to file...\n");CHKERRQ(ierr);
    ierr = networkWrite(par_ptr->outputNetwork, box_ptr);CHKERRQ(ierr);
    ierr = destroyBox(box_ptr);CHKERRQ(ierr);
    ierr = destroyParameters(par_ptr);CHKERRQ(ierr);
    ierr = PetscLogStagePop();CHKERRQ(ierr);

    /*
	 * Free work space.
	 */
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Cleaning up...\n");CHKERRQ(ierr);
	ierr = VecDestroy(&x);CHKERRQ(ierr); ierr = VecDestroy(&u);CHKERRQ(ierr);
	ierr = VecDestroy(&b);CHKERRQ(ierr); ierr = MatDestroy(&A);CHKERRQ(ierr);
	ierr = KSPDestroy(&ksp);CHKERRQ(ierr);

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Finalising...\n");CHKERRQ(ierr);
	ierr = PetscFinalize();

	return ierr;
}
