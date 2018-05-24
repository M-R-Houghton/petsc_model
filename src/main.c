
static char help[] = "The first draft of the PETSc model.\n\n";

#include "networkWrite.h"
#include "unitTests.h"

int main(int argc, char **args)
{
	PetscErrorCode ierr;
	ierr = PetscInitialize(&argc,&args,(char*)0,help);if (ierr) return ierr;

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

	ierr = PetscFinalize();
	return ierr;
}