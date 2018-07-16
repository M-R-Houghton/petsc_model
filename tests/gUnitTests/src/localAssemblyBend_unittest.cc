#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "localAssemblyBend.h"
}


namespace {

TEST(testGoogleTestImplementation, testPetscCommand) 
{
    PetscErrorCode ierr;
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Setting up vectors...\n");
    EXPECT_EQ(ierr, 0);
}

TEST(testCalculateKappa, testOutputKappaValue) 
{
    Parameters *par_ptr         = (Parameters *)malloc(sizeof(Parameters));
    par_ptr->youngsModulus        = 1.0/(M_PI * pow(0.01, 2));

    PetscInt fCount             = 2;
    Box *box_ptr              = (Box *)malloc(sizeof(Box));
    box_ptr->fibreCount           = fCount;
    box_ptr->masterFibreList        = (Fibre*)calloc(fCount, sizeof(Fibre));
    box_ptr->masterFibreList[0].fibreID = 0;
    box_ptr->masterFibreList[0].radius  = 0.01;
    box_ptr->masterFibreList[1].fibreID = 1;
    box_ptr->masterFibreList[1].radius  = 0.3;

    EXPECT_DOUBLE_EQ(calculateKappa(box_ptr, par_ptr, 0), 0.000025);
    EXPECT_DOUBLE_EQ(calculateKappa(box_ptr, par_ptr, 1), 20.25);

    /* clean up */
    free(par_ptr); par_ptr = NULL;
    free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
}

} /* namespace */
