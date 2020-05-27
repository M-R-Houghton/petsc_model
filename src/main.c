
static char help[] = "The first draft of the PETSc model.\n\n";

#include "networkWrite.h"
#include "integrationTests.h"

int main(int argc, char **args)
{
    Box             *box_ptr;
    Parameters      *par_ptr;
    Vec             vecB, vecU, vecX;       /* approx solution, RHS, exact solution */
    Mat             matH;                   /* linear system matrix */
    PetscReal       norm;                   /* norm of solution error */
    PetscErrorCode  ierr;
    PetscInt        n,N;
    PetscMPIInt     size;
    //PetscScalar     one = 1.0;
    PetscBool       nonzeroguess = PETSC_FALSE;
    PetscViewer     viewer;
    
    /* set up default values for general params */
    PetscScalar     gamma = 0.01;
    PetscScalar     yMod = 1.0;
    
    /* set EM and TS default values */
    PetscBool       useKSP = PETSC_TRUE;
    PetscBool       useEM = PETSC_FALSE;
    PetscScalar     lambda = 1e-5;       
    PetscBool       useTS = PETSC_FALSE;
    PetscScalar     alpha = 1e-1;
    PetscScalar     normTolF = 1e-12;
    PetscInt        maxSteps = 10000000;
    PetscBool       restartTS = PETSC_TRUE;

#if defined(PETSC_USE_LOG)
    PetscLogStage stages[6];
#endif

    // testing the use of tag update

    if (argc < 2)
    {
        printf("[ERROR] Incorrect number of arguments provided.\n");
        exit(EXIT_FAILURE);
    }
    
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            Model set up
        - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    /* set options file */
    const char optFile[] = "modelOptions.dat";
    
    /* set parameter file */
    char *parFile = args[1];
    
    printf("[STATUS] Initialising...\n");
    ierr = PetscInitialize(&argc,&args,optFile,help);if (ierr) return ierr;
    ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);CHKERRQ(ierr);
    if (size != 1) SETERRQ(PETSC_COMM_WORLD,1,"This is a uniprocessor example only!");

    /* 
        START: to be put in own function 
    */
    ierr = PetscOptionsGetInt(NULL,NULL,"-n",&n,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetBool(NULL,NULL,"-nonzero_guess",&nonzeroguess,NULL);CHKERRQ(ierr);
    
    /* set up options for general usage */
    ierr = PetscOptionsGetReal(NULL,NULL,"-gamma",&gamma,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetReal(NULL,NULL,"-y_mod",&yMod,NULL);CHKERRQ(ierr);
    /* set up options for elastic medium */
    ierr = PetscOptionsGetBool(NULL,NULL,"-use_em",&useEM,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetReal(NULL,NULL,"-k",&lambda,NULL);CHKERRQ(ierr);
    
    /* set up options for solving with KSP */
    ierr = PetscOptionsGetBool(NULL,NULL,"-use_ksp",&useKSP,NULL);CHKERRQ(ierr);
    
    /* set up options for time stepping */
    ierr = PetscOptionsGetBool(NULL,NULL,"-use_ts",&useTS,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetBool(NULL,NULL,"-ts_restart",&restartTS,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetInt(NULL,NULL,"-max_steps",&maxSteps,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetReal(NULL,NULL,"-alpha",&alpha,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetReal(NULL,NULL,"-f_tol",&normTolF,NULL);CHKERRQ(ierr);
    /* 
        END: to be put in own function 
    */
    
    /* perform all unit tests */
    //ierr = runIntegrationTests();CHKERRQ(ierr);
    
    /* Register stages for separate profiling */
    ierr = PetscLogStageRegister("Network Read-in",  &stages[0]);CHKERRQ(ierr);
    ierr = PetscLogStageRegister("System Assembly",  &stages[1]);CHKERRQ(ierr);
    ierr = PetscLogStageRegister("System Solve",     &stages[2]);CHKERRQ(ierr);
    ierr = PetscLogStageRegister("Network Update",   &stages[3]);CHKERRQ(ierr);
    ierr = PetscLogStageRegister("Network Analysis", &stages[4]);CHKERRQ(ierr);
    ierr = PetscLogStageRegister("Network Write-out",&stages[5]);CHKERRQ(ierr);
    
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            Network read in
        - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading parameter file...\n");CHKERRQ(ierr);
    ierr = parameterRead(parFile, &par_ptr, gamma, yMod);CHKERRQ(ierr);
    
    /* read in network data file */
    ierr = PetscLogStagePush(stages[0]);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading from file '%s'...\n", par_ptr->inputNetwork);CHKERRQ(ierr);
    ierr = networkRead(par_ptr->inputNetwork, &box_ptr, par_ptr->gamma);CHKERRQ(ierr);
    N    = box_ptr->nodeInternalCount;
    n    = N * DIMENSION;
    /* WARNING: here we are overwriting the number of procs n... why? */
    // TODO: change this so that we have separate variables.
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Problem size is (%d x %d)\n", n, n);CHKERRQ(ierr);
    ierr = PetscLogStagePop();CHKERRQ(ierr);
    
    /* simple check to make sure macro is correctly set */
    if (box_ptr->xyzDimension[2] == 0.0) assert(DIMENSION == 2);
    if (box_ptr->xyzDimension[2] != 0.0) assert(DIMENSION == 3);
    
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            System assembly
        - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    /* allocate memory for global rhs and solution vectors and set them up */
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting up vectors...\n");CHKERRQ(ierr);
    ierr = VecCreate(PETSC_COMM_WORLD,&vecB);CHKERRQ(ierr);
    //ierr = PetscObjectSetName((PetscObject) vecB, "Solution");CHKERRQ(ierr);
    ierr = VecSetSizes(vecB,PETSC_DECIDE,n);CHKERRQ(ierr);
    ierr = VecSetFromOptions(vecB);CHKERRQ(ierr);
    ierr = VecDuplicate(vecB,&vecU);CHKERRQ(ierr);
    ierr = VecDuplicate(vecB,&vecX);CHKERRQ(ierr);
    
    /* allocate memory for global matrix and set it up */
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting up matrix...\n");CHKERRQ(ierr);
    ierr = MatCreate(PETSC_COMM_WORLD,&matH);CHKERRQ(ierr);
    ierr = MatSetSizes(matH,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);
    ierr = MatSetFromOptions(matH);CHKERRQ(ierr);
    ierr = MatSetUp(matH);CHKERRQ(ierr);
    
    /* assemble sparse structure and assemble linear system */
    ierr = PetscLogStagePush(stages[1]);CHKERRQ(ierr);
    ierr = systemAssembly(box_ptr,par_ptr,matH,vecB);CHKERRQ(ierr);
    if (useEM)
    {
        //ierr = applyElasticMedium(box_ptr, matH, vecB, lambda);CHKERRQ(ierr);
    }
    ierr = PetscLogStagePop();CHKERRQ(ierr);
    
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            Solve the linear system
        - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    ierr = PetscLogStagePush(stages[2]);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Solving system...\n");CHKERRQ(ierr);
    /* solve linear system via matrix inversion */
    if (useKSP)
    {
        ierr = systemSolve(matH,vecB,vecU);CHKERRQ(ierr);
    }
    
    /* solve the linear system via time stepping */
    if (useTS)
    {
        /* set initial U and begin time stepping */
        ierr = VecSet(vecX, 0.0);CHKERRQ(ierr);
    
        if (restartTS)
        {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting initial U to U_aff...\n");CHKERRQ(ierr);
            ierr = assembleAffineDisplacementVector(box_ptr, vecX);CHKERRQ(ierr);
        }
        else 
        {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading vector.dat into initial U...\n");CHKERRQ(ierr);
            ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,"vector.dat",FILE_MODE_READ,&viewer);CHKERRQ(ierr);
            ierr = VecCreate(PETSC_COMM_WORLD,&vecX);CHKERRQ(ierr);
            ierr = VecLoad(vecX,viewer);CHKERRQ(ierr);
            ierr = PetscViewerDestroy(&viewer);CHKERRQ(ierr);
        }
    
        ierr = systemTimeStepSolve(matH,vecB,vecX,alpha,normTolF,maxSteps);CHKERRQ(ierr);
    
        /* check the error if using both methods */
        if (useKSP)
        {
            ierr = VecAXPY(vecU,-1.0,vecX);CHKERRQ(ierr);
            ierr = VecNorm(vecU,NORM_2,&norm);CHKERRQ(ierr);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Norm of error against LU = %g\n",(double)norm);
            CHKERRQ(ierr);
        }
    
        /* beware that copy may need modifying in parallel */
        ierr = VecCopy(vecX, vecU);
    }
    
    /* check for no solving technique selection */
    if (!useKSP && !useTS)
    {
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[ERROR] Need to solve with KSP or TS");CHKERRQ(ierr); 
    }
    ierr = PetscLogStagePop();CHKERRQ(ierr);
    
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            Update the network and analyse
        - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    ierr = PetscLogStagePush(stages[3]);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Updating network...\n");CHKERRQ(ierr);
    ierr = networkUpdate(box_ptr,vecU);CHKERRQ(ierr);
    ierr = PetscLogStagePop();CHKERRQ(ierr);
    
    /* make predictions based on solution */
    ierr = PetscLogStagePush(stages[4]);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Analysing network...\n");CHKERRQ(ierr);
    ierr = networkAnalysis(box_ptr, par_ptr);CHKERRQ(ierr);
    ierr = printAnalysis(box_ptr, par_ptr);CHKERRQ(ierr);
    ierr = writeAnalysis(box_ptr, par_ptr);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tLambda \t= %g\n\n", lambda);CHKERRQ(ierr);
    ierr = PetscLogStagePop();CHKERRQ(ierr);
    
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            Network write out to file
        - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    ierr = PetscLogStagePush(stages[5]);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Writing to file...\n");CHKERRQ(ierr);
    ierr = networkWrite(par_ptr->outputNetwork, par_ptr->outadvNetwork, box_ptr);CHKERRQ(ierr);
    ierr = destroyBox(box_ptr);CHKERRQ(ierr);
    ierr = destroyParameters(par_ptr);CHKERRQ(ierr);
    ierr = PetscLogStagePop();CHKERRQ(ierr);
    
    /*
     * Clean up Petsc objects
     */
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Cleaning up...\n");CHKERRQ(ierr);
    ierr = VecDestroy(&vecB);CHKERRQ(ierr); ierr = VecDestroy(&vecX);CHKERRQ(ierr);
    ierr = VecDestroy(&vecU);CHKERRQ(ierr); ierr = MatDestroy(&matH);CHKERRQ(ierr);
    
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Finalising...\n");CHKERRQ(ierr);
    ierr = PetscFinalize();
    
    return ierr;
}
