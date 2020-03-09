#include "parameterRead.h"

void checkFileNameLengths(const char *inputFileName, const char *outputFileName,
                            const char *outadvFileName, const char *resultsFileName)
{
    /* adjust MAX_NAME in common.h if this fails */
    assert(strlen(inputFileName)   < MAX_NAME);
    assert(strlen(outputFileName)  < MAX_NAME);
    assert(strlen(outadvFileName)  < MAX_NAME);
    assert(strlen(resultsFileName) < MAX_NAME);
}


/* Initiates parameter file read in routine */
PetscErrorCode parameterRead(const char *fileToRead_ptr, Parameters **par_ptr,
                                const PetscScalar gamma, const PetscScalar youngsModulus)
{
    PetscErrorCode  ierr = 0;
    //PetscScalar   gamma, youngsModulus;
    FILE            *file_ptr;
    char            inputNetwork[MAX_NAME];
    char            outputNetwork[MAX_NAME];
    char            outadvNetwork[MAX_NAME];
    char            postSolveResults[MAX_NAME];

    /* open file and check whether successful */
    file_ptr = fopen(fileToRead_ptr, "r");
    if (file_ptr == NULL) SETERRQ(PETSC_COMM_WORLD,65,"Error in opening file.");

    /* scan both lines at once */
    fscanf(file_ptr, " %s\n %s\n %s\n %s\n", inputNetwork, outputNetwork, outadvNetwork, postSolveResults);

    /* filenames should be less than macro for max length */
    checkFileNameLengths(inputNetwork, outputNetwork, outadvNetwork, postSolveResults);

    /* use obtained information to set up parameters */
    *par_ptr = makeParameters(inputNetwork, outputNetwork, outadvNetwork, postSolveResults, gamma, youngsModulus);

    /* close file */
    fclose(file_ptr);

    return ierr;
}


