
static char help[] = "The first draft of the PETSc model.\n\n";

#include "networkWrite.h"
#include "unitTests.h"

int main(int argc, char **args)
{
	Vec            x, b, u;      /* approx solution, RHS, exact solution */
  	Mat            A;            /* linear system matrix */
  	KSP            ksp;          /* linear solver context */
	PC             pc;           /* preconditioner context */
  	PetscReal      norm;         /* norm of solution error */
	PetscErrorCode ierr;
	PetscInt       i,n = 10,col[3],its;
	PetscMPIInt    size;
	PetscScalar    one = 1.0,value[3];
	PetscBool      nonzeroguess = PETSC_FALSE,changepcside = PETSC_FALSE;

	printf("[STATUS] Initialising...\n");
	ierr = PetscInitialize(&argc,&args,(char*)0,help);if (ierr) return ierr;
	ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);CHKERRQ(ierr);
  	if (size != 1) SETERRQ(PETSC_COMM_WORLD,1,"This is a uniprocessor example only!");
	ierr = PetscOptionsGetInt(NULL,NULL,"-n",&n,NULL);CHKERRQ(ierr);
	ierr = PetscOptionsGetBool(NULL,NULL,"-nonzero_guess",&nonzeroguess,NULL);CHKERRQ(ierr);

	// run unit tests
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Running tests...\n");CHKERRQ(ierr);
	ierr = runUnitTests(ierr);

	// read in network data file
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading from file...\n");CHKERRQ(ierr);
	ierr = networkRead();

	// assemble sparse structure and assemble linear system
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Assembling system...\n");CHKERRQ(ierr);
	ierr = systemAssembly();

	// minimal PETSc assembly example
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting up vectors...\n");CHKERRQ(ierr);
	ierr = VecCreate(PETSC_COMM_WORLD,&x);CHKERRQ(ierr);
	//ierr = PetscObjectSetName((PetscObject) x, "Solution");CHKERRQ(ierr);
	ierr = VecSetSizes(x,PETSC_DECIDE,n);CHKERRQ(ierr);
	ierr = VecSetFromOptions(x);CHKERRQ(ierr);
	ierr = VecDuplicate(x,&b);CHKERRQ(ierr);
	ierr = VecDuplicate(x,&u);CHKERRQ(ierr);

	ierr = VecView(x,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	ierr = VecView(b,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	ierr = VecView(u,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting up matrix...\n");CHKERRQ(ierr);
	ierr = MatCreate(PETSC_COMM_WORLD,&A);CHKERRQ(ierr);
	ierr = MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);
	ierr = MatSetFromOptions(A);CHKERRQ(ierr);
	ierr = MatSetUp(A);CHKERRQ(ierr);

	/*
	  Assemble matrix
	*/
	value[0] = -1.0; value[1] = 2.0; value[2] = -1.0;
	for(i=1; i<n-1; i++) 
	{
		col[0] = i-1; col[1] = i; col[2] = i+1;
		ierr   = MatSetValues(A,1,&i,3,col,value,INSERT_VALUES);CHKERRQ(ierr);
	}
	i    = n - 1; col[0] = n - 2; col[1] = n - 1;
	ierr = MatSetValues(A,1,&i,2,col,value,INSERT_VALUES);CHKERRQ(ierr);
	i    = 0; col[0] = 0; col[1] = 1; value[0] = 2.0; value[1] = -1.0;
	ierr = MatSetValues(A,1,&i,2,col,value,INSERT_VALUES);CHKERRQ(ierr);
	ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

	ierr = MatView(A,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);


	/*
       Set exact solution; then compute right-hand-side vector.
    */
    ierr = VecSet(u,one);CHKERRQ(ierr);
    ierr = MatMult(A,u,b);CHKERRQ(ierr);

    ierr = VecView(u,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    ierr = VecView(b,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);

	ierr = KSPSetOperators(ksp,A,A);CHKERRQ(ierr);

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

    if (nonzeroguess) 
    {
		PetscScalar p = .5;
		ierr = VecSet(x,p);CHKERRQ(ierr);
		ierr = KSPSetInitialGuessNonzero(ksp,PETSC_TRUE);CHKERRQ(ierr);
	}

	// solve linear system
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Solving system...\n");CHKERRQ(ierr);
    ierr = systemSolve();


    /*
	  Solve linear system
	*/
	ierr = KSPSolve(ksp,b,x);CHKERRQ(ierr);

	/*
	  View solver info; we could instead use the option -ksp_view to
	  print this info to the screen at the conclusion of KSPSolve().
	*/
	ierr = KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	ierr = VecView(x,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	ierr = VecView(b,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);


	/*
      Check the error
    */
    ierr = VecAXPY(x,-1.0,u);CHKERRQ(ierr);
    ierr = VecNorm(x,NORM_2,&norm);CHKERRQ(ierr);
    ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm of error %g, Iterations %D\n",(double)norm,its);CHKERRQ(ierr);

    // make predictions based on solution
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Analysing network...\n");CHKERRQ(ierr);
    ierr = networkAnalysis();

    // write out new network data file
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Writing to file...\n");CHKERRQ(ierr);
    ierr = networkWrite();

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
