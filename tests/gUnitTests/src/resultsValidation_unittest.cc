#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "resultsValidation.h"
}


namespace {

struct testTripodResults : ::testing::Test
{
    char const *oldFile;
    char const *newFile;

    FILE *fp_old;
    FILE *fp_new;

    char lineOld[MAX_LENGTH], *lineOld_ptr;
    char lineNew[MAX_LENGTH], *lineNew_ptr;

    void SetUp()
    {
        oldFile = "../../../serial_3d_model/dat_files/f3t/f3tTripod1_out.dat";
        newFile = "../../data/dat/f3tTripod1_out.dat";

        fp_old = fopen(oldFile, "r");
        ASSERT_FALSE(fp_old == NULL);

        fp_new = fopen(newFile, "r");
        ASSERT_FALSE(fp_new == NULL);
    }

    void TearDown()
    {
        fclose(fp_new);
        fclose(fp_old);
    }
};


TEST_F(testTripodResults, testFileEquality) {

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}

} /* namespace */
