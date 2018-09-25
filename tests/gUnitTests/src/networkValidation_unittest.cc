#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "networkValidation.h"
}


namespace {

struct testFileEquality : ::testing::Test
{
    char const *oldFile;
    char const *newFile;

    FILE *fp_old;
    FILE *fp_new;

    char lineOld[MAX_LENGTH], *lineOld_ptr;
    char lineNew[MAX_LENGTH], *lineNew_ptr;

    void SetUp()
    {

    }

    void TearDown()
    {
        fclose(fp_new);
        fclose(fp_old);
    }
};


TEST_F(testFileEquality, testTripod1Results) 
{
    oldFile = "../../data/dat/ref/f3tTripod1_out.dat";
    newFile = "../../data/dat/tri/tri_3d_01_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testTripod2Results) 
{
    oldFile = "../../data/dat/ref/f3tTripod2_out.dat";
    newFile = "../../data/dat/tri/tri_3d_02_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testTripod3Results) 
{
    oldFile = "../../data/dat/ref/f3tTripod3_out.dat";
    newFile = "../../data/dat/tri/tri_3d_03_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testTripod4Results) 
{
    oldFile = "../../data/dat/ref/f3tTripod4_out.dat";
    newFile = "../../data/dat/tri/tri_3d_04_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testTripodAdd1Results) 
{
    oldFile = "../../data/dat/ref/f3tLoose1_out.dat";
    newFile = "../../data/dat/tri/tri_3d_add_01_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testTripodAdd2Results) 
{
    oldFile = "../../data/dat/ref/f3tLoose2_out.dat";
    newFile = "../../data/dat/tri/tri_3d_add_02_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testTripodAdd3Results) 
{
    oldFile = "../../data/dat/ref/f3tLoose3_out.dat";
    newFile = "../../data/dat/tri/tri_3d_add_03_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testTripodAdd4Results) 
{
    oldFile = "../../data/dat/ref/f3tLoose4_out.dat";
    newFile = "../../data/dat/tri/tri_3d_add_04_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testLambdaResults) 
{
    oldFile = "../../data/dat/ref/lmbDefault_out.dat";
    newFile = "../../data/dat/lmb/lmb_2d_01_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testModifiedLambdaResults) 
{
    oldFile = "../../data/dat/ref/lmbDefaultMod_out.dat";
    newFile = "../../data/dat/lmb/lmb_2d_02_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testLeftShiftLambdaResults) 
{
    oldFile = "../../data/dat/ref/lmbLShift_out.dat";
    newFile = "../../data/dat/lmb/lmb_2d_lshft_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testRightShiftLambdaResults) 
{
    oldFile = "../../data/dat/ref/lmbRShift_out.dat";
    newFile = "../../data/dat/lmb/lmb_2d_rshft_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testLambdaAdd1Results) 
{
    oldFile = "../../data/dat/ref/lmbAdd1_out.dat";
    newFile = "../../data/dat/lmb/lmb_2d_add_01_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testLambdaAdd2Results) 
{
    oldFile = "../../data/dat/ref/lmbAdd2_out.dat";
    newFile = "../../data/dat/lmb/lmb_2d_add_02_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testLambdaAdd3Results) 
{
    oldFile = "../../data/dat/ref/lmbAdd3_out.dat";
    newFile = "../../data/dat/lmb/lmb_2d_add_03_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, testLambdaAdd4Results) 
{
    oldFile = "../../data/dat/ref/lmbAdd4_out.dat";
    newFile = "../../data/dat/lmb/lmb_2d_add_04_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, DISABLED_testLattice2DResults) 
{
    oldFile = "../../data/dat/ref/dil_0_94_out.dat";
    newFile = "../../data/dat/lat2D/dil_0_94_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, DISABLED_testLattice3DResults) 
{
    oldFile = "../../data/dat/ref/f3tLat02_out.dat";
    newFile = "../../data/dat/lat3D/lat_02_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}


TEST_F(testFileEquality, DISABLED_testRandom2DResults) 
{
    oldFile = "../../data/dat/ref/rnd01_out.dat";
    newFile = "../../data/dat/rnd2D/rnd01_out.dat";

    fp_old = fopen(oldFile, "r");
    ASSERT_FALSE(fp_old == NULL);

    fp_new = fopen(newFile, "r");
    ASSERT_FALSE(fp_new == NULL);

    do {
        lineOld_ptr = fgets(lineOld, sizeof(lineOld), fp_old);
        lineNew_ptr = fgets(lineNew, sizeof(lineNew), fp_new);
        
        EXPECT_STREQ(lineOld_ptr, lineNew_ptr);
    } while (lineOld_ptr != NULL && lineNew_ptr != NULL);
}

} /* namespace */
