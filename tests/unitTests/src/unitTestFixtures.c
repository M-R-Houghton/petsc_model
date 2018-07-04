#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "unitTestFixtures.h"

void setup(void) {
	//cr_error("Runs before the test");
	/*
	PetscErrorCode ierr;
	PetscMPIInt    size;
	int argc = 4;
	const char *args[4]; 
	args[0] = "mpiexec";
	args[1] = "-n";
	args[2] = "1";
	args[3] = "./cUnitTests";
	ierr = PetscInitialize(&argc,&args,(char*)0,help);
	ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);
	*/
}

//int argc, char **args

void teardown(void) {
	//cr_log_error("Runs after the test");
	/*
	PetscErrorCode ierr;
	ierr = PetscFinalize();
	*/
}
