#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C"
{
    #include "parameterRead.h"
}


namespace {


struct testParameterRead : ::testing::Test
{
    Parameters  *par_ptr;
    const char *fileToRead;
    const PetscScalar myGamma = 0.05; 
    const PetscScalar myYM = 1.0; 

    void SetUp()
    {
        // open file, read in line and close
        fileToRead = "../../data/par/tri/tri_3d_01.par";
    }

    void TearDown()
    {
        destroyParameters(par_ptr);
    }
};


TEST_F(testParameterRead, testErrorOutput)
{
    EXPECT_EQ(parameterRead(fileToRead, &par_ptr, myGamma, myYM), 0);
}


TEST_F(testParameterRead, testOutputValues)
{
    const char input[]  = "data/dat/tri/tri_3d_01_in.dat";
    const char output[] = "data/dat/tri/tri_3d_01_out.dat";
    parameterRead(fileToRead, &par_ptr, myGamma, myYM);

    EXPECT_STREQ(par_ptr->inputNetwork,     input);
    EXPECT_STREQ(par_ptr->outputNetwork,    output);
    EXPECT_EQ(par_ptr->gamma,               myGamma);
    EXPECT_EQ(par_ptr->youngsModulus,       myYM);
}

} /* namespace */
