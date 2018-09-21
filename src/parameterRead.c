#include "parameterRead.h"

/* Initiates parameter file read in routine */
PetscErrorCode parameterRead(const char *fileToRead_ptr, Parameters **par_ptr)
{
	PetscErrorCode 	ierr;
	PetscScalar 	gamma, youngsModulus;
	FILE 			*file_ptr;
	char 			inputNetwork[MAX_NAME];
	char 			outputNetwork[MAX_NAME];

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading parameter file...\n");CHKERRQ(ierr);

	/* these may be added in the parameter files later */
	gamma 			= GAMMA;
	youngsModulus 	= YOUNGS_MOD;

	/* open file and check whether successful */
	file_ptr = fopen(fileToRead_ptr, "r");
	if (file_ptr == NULL) SETERRQ(PETSC_COMM_WORLD,65,"Error in opening file.");

	/* scan both lines at once */
	fscanf(file_ptr, " %s\n %s\n", inputNetwork, outputNetwork);
   
   	/* use these to check for unwanted whitespace */
   	printf("Read input file  |%s|\n", inputNetwork  );
   	printf("Read output file |%s|\n", outputNetwork );


   	/* use obtained information to set up parameters */
   	*par_ptr = makeParameters(inputNetwork, outputNetwork, gamma, youngsModulus);

	/* close file */
	fclose(file_ptr);

	return ierr;
}


