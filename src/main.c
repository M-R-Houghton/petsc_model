
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
	PetscMPIInt    size;

	ierr = PetscInitialize(&argc,&args,(char*)0,help);if (ierr) return ierr;
	ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);CHKERRQ(ierr);
  	if (size != 1) SETERRQ(PETSC_COMM_WORLD,1,"This is a uniprocessor example only!");

	// run unit tests
	ierr = runUnitTests(ierr);

	// read in network data file
	ierr = networkRead();

	// assemble sparse structure and assemble linear system
	ierr = systemAssembly();

	// solve linear system 
    ierr = systemSolve();

    // make predictions based on solution
    ierr = networkAnalysis();

    // write out new network data file
    ierr = networkWrite();

    /*
	 * Free work space.
	 */
	ierr = VecDestroy(&x);CHKERRQ(ierr); ierr = VecDestroy(&u);CHKERRQ(ierr);
	ierr = VecDestroy(&b);CHKERRQ(ierr); ierr = MatDestroy(&A);CHKERRQ(ierr);
	ierr = KSPDestroy(&ksp);CHKERRQ(ierr);

	ierr = PetscFinalize();
	return ierr;
}
