#include <petscksp.h>
#include <criterion/criterion.h>
#include "networkBuild.h"

static char help[] = "Unit testing suite\n\n";

void setup(int argc, char **args) {
	PetscErrorCode ierr;
	PetscMPIInt    size;
	ierr = PetscInitialize(&argc,&args,(char*)0,help);
	ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);
}

void teardown(void) {
	PetscErrorCode ierr;
	ierr = PetscFinalize();
}