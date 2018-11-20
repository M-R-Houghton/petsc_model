
static char help[] = "The first draft of the PETSc model.\n\n";

#include "networkWrite.h"
#include "integrationTests.h"

int main(int argc, char **args)
{
	Box  		    *box_ptr;
	Parameters 	    *par_ptr;
	Vec             vecB, vecU, vecX;       /* approx solution, RHS, exact solution */
  	Mat             matH;                   /* linear system matrix */
  	//KSP            ksp;          
	//PC             pc;          
  	PetscReal       norm;                   /* norm of solution error */
	PetscErrorCode  ierr;
	PetscInt        n,N;
	//PetscInt 	    its;
	PetscMPIInt     size;
	PetscScalar     one = 1.0;
	PetscBool       nonzeroguess = PETSC_FALSE;
	//PetscBool 	    changepcside = PETSC_FALSE;

	PetscScalar     lambda   = -1e-5;       /* set EM and TS default values */
    PetscScalar     alpha    = ALPHA;
    PetscScalar     normTolF = F_TOL;
    PetscInt        maxSteps = MAX_STEPS;

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
    
    /* set up options for elastic medium */
    ierr = PetscOptionsGetReal(NULL,NULL,"-k",&lambda,NULL);CHKERRQ(ierr);

    /* set up options for time stepping */
    ierr = PetscOptionsGetInt(NULL,NULL,"-max_steps",&maxSteps,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetReal(NULL,NULL,"-alpha",&alpha,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetReal(NULL,NULL,"-f_tol",&normTolF,NULL);CHKERRQ(ierr);

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
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading from file '%s'...\n", par_ptr->inputNetwork);CHKERRQ(ierr);
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
         	the linear system, matHvecB = vecU.
     	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* allocate memory for global rhs and solution vectors and set them up */
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting up vectors...\n");CHKERRQ(ierr);
	ierr = VecCreate(PETSC_COMM_WORLD,&vecB);CHKERRQ(ierr);
	//ierr = PetscObjectSetName((PetscObject) vecB, "Solution");CHKERRQ(ierr);
	ierr = VecSetSizes(vecB,PETSC_DECIDE,n);CHKERRQ(ierr);
	ierr = VecSetFromOptions(vecB);CHKERRQ(ierr);
	ierr = VecDuplicate(vecB,&vecU);CHKERRQ(ierr);
	ierr = VecDuplicate(vecB,&vecX);CHKERRQ(ierr);

	//ierr = VecView(vecB,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	//ierr = VecView(vecU,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	//ierr = VecView(vecX,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/* allocate memory for global matrix and set it up */
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting up matrix...\n");CHKERRQ(ierr);
	ierr = MatCreate(PETSC_COMM_WORLD,&matH);CHKERRQ(ierr);
	ierr = MatSetSizes(matH,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);
	ierr = MatSetFromOptions(matH);CHKERRQ(ierr);
	ierr = MatSetUp(matH);CHKERRQ(ierr);

	/* assemble sparse structure and assemble linear system */
	ierr = PetscLogStagePush(stages[1]);CHKERRQ(ierr);
	ierr = systemAssembly(box_ptr,par_ptr,matH,vecB);CHKERRQ(ierr);
	//ierr = applyElasticMedium(box_ptr, matH, vecB, lambda);CHKERRQ(ierr);
	ierr = PetscLogStagePop();CHKERRQ(ierr);

	/*
       Set exact solution; then compute right-hand-side vector.
    */
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting exact solution...\n");CHKERRQ(ierr);
    //ierr = VecSet(vecX,one);CHKERRQ(ierr);
    //ierr = MatMult(matH,vecX,vecU);CHKERRQ(ierr);

    //ierr = VecView(vecB,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    //ierr = VecView(vecU,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

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
	//ierr = KSPSetOperators(ksp,matH,matH);CHKERRQ(ierr);

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
		ierr = VecSet(vecB,p);CHKERRQ(ierr);
		ierr = KSPSetInitialGuessNonzero(ksp,PETSC_TRUE);CHKERRQ(ierr);
	}
	*/

	/* solve linear system */
	ierr = PetscLogStagePush(stages[2]);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Solving system...\n");CHKERRQ(ierr);
    //ierr = systemSolve(matH,vecB,vecU);CHKERRQ(ierr);

    /* set initial U and begin time stepping */
    ierr = VecSet(vecX, 0.0);CHKERRQ(ierr);
    PetscInt i,j;

	for (i = 0; i < box_ptr->nodeCount; i++)
	{
		Node *node = &(box_ptr->masterNodeList[i]);
		if (node->globalID != -1)
		{
			for (j = 0; j < DIMENSION; j++)
			{
				ierr = VecSetValue(vecX, node->globalID + j*N, node->xyzAffDisplacement[j], ADD_VALUES);
				CHKERRQ(ierr);
			}
		}
	}
    ierr = systemTimeStepSolve(matH,vecB,vecX,alpha,normTolF,maxSteps);CHKERRQ(ierr);

    /* check the error *//*
    ierr = VecAXPY(vecU,-1.0,vecX);CHKERRQ(ierr);
    ierr = VecNorm(vecU,NORM_2,&norm);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Norm of error %g\n",(double)norm);CHKERRQ(ierr);

    *//* beware that copy may need modifying in parallel */
    ierr = VecCopy(vecX, vecU);
    ierr = PetscLogStagePop();CHKERRQ(ierr);

    ierr = PetscLogStagePush(stages[3]);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Updating network...\n");CHKERRQ(ierr);
    ierr = networkUpdate(box_ptr,vecU);CHKERRQ(ierr);
    ierr = PetscLogStagePop();CHKERRQ(ierr);


    /* assemble tripod matrix */
    /*
    char rowFile[100] = "data/row/row.f3tTripod1";
    char colFile[100] = "data/col/col.f3tTripod1";
    char matFile[100] = "data/mat/mat.f3tTripod1";
    char rhsFile[100] = "data/rhs/rhs.f3tTripod1";
    char solFile[100] = "data/sol/sol.f3tTripod1";
    ierr = solveAssembledMatrix(rowFile,colFile,matFile,rhsFile,solFile,3);CHKERRQ(ierr);
    */

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            Solve the linear system
     	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    /*
	  Solve linear system
	*/
	//ierr = KSPSolve(ksp,vecB,vecU);CHKERRQ(ierr);

	/*
	  View solver info; we could instead use the option -ksp_view to
	  print this info to the screen at the conclusion of KSPSolve().
	*/
	//ierr = KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	//ierr = VecView(vecB,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	//ierr = VecView(vecU,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        	Check solution and clean up
     	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/*
      Check the error
    */
    /*
    ierr = VecAXPY(vecB,-1.0,vecX);CHKERRQ(ierr);
    ierr = VecNorm(vecB,NORM_2,&norm);CHKERRQ(ierr);
    ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm of error %g, Iterations %D\n",(double)norm,its);CHKERRQ(ierr);
	*/

    /* make predictions based on solution */
    ierr = PetscLogStagePush(stages[4]);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Analysing network...\n");CHKERRQ(ierr);
    ierr = networkAnalysis(box_ptr, par_ptr);CHKERRQ(ierr);
    ierr = printAnalysis(box_ptr, par_ptr);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tLambda \t= %g\n\n", lambda);CHKERRQ(ierr);


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
	ierr = VecDestroy(&vecB);CHKERRQ(ierr); ierr = VecDestroy(&vecX);CHKERRQ(ierr);
	ierr = VecDestroy(&vecU);CHKERRQ(ierr); ierr = MatDestroy(&matH);CHKERRQ(ierr);
	//ierr = KSPDestroy(&ksp);CHKERRQ(ierr);

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Finalising...\n");CHKERRQ(ierr);
	ierr = PetscFinalize();

	return ierr;
}
