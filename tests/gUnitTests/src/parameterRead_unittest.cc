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

    void SetUp()
    {
        // open file, read in line and close
        fileToRead = "../../data/par/tri_3d_01.par";
    }

    void TearDown()
    {
        destroyParameters(par_ptr);
    }
};


TEST_F(testParameterRead, testErrorOutput)
{
    EXPECT_EQ(parameterRead(fileToRead, &par_ptr), 0);
}


TEST_F(testParameterRead, testOutputValues)
{
    const char input[]  = "data/dat/tri/tri_3d_01_in.dat";
    const char output[] = "data/dat/tri/tri_3d_01_out.dat";
    parameterRead(fileToRead, &par_ptr);

    EXPECT_STREQ(par_ptr->inputNetwork,     input);
    EXPECT_STREQ(par_ptr->outputNetwork,    output);
    EXPECT_EQ(par_ptr->gamma,               GAMMA);
    EXPECT_EQ(par_ptr->youngsModulus,       YOUNGS_MOD);
}

} /* namespace */
