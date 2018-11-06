#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C"
{
    #include "networkAnalysis.h"
}


namespace {

struct testNetworkAnalysis : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 0.05, 1.0);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
    }
};


TEST_F(testNetworkAnalysis, testErrorOutput)
{
    EXPECT_EQ(networkAnalysis(box_ptr, par_ptr), 0);
}


struct testCalculateSegStretchEnergy : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    PetscInt fIndex;
    Node *alph_ptr;
    Node *omeg_ptr;
    Node *beta_ptr;
    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 0.05, 1.0);

        alph_ptr = &(box_ptr->masterNodeList[0]);
        beta_ptr = &(box_ptr->masterNodeList[4]);
        /*
        alpha_ptr->xyzDisplacement[0] = 0;
        alpha_ptr->xyzDisplacement[1] = 0;
        alpha_ptr->xyzDisplacement[2] = 0;
        */
        beta_ptr->xyzDisplacement[0] = 0.01250598041564164;
        beta_ptr->xyzDisplacement[1] = -0.006248884007600541;
        beta_ptr->xyzDisplacement[2] = 0;

        fIndex = 0;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
    }
};


TEST_F(testCalculateSegStretchEnergy, testOutputValueNodes04)
{
    EXPECT_DOUBLE_EQ(calculateSegStretchEnergy( box_ptr, par_ptr, fIndex, 
                                                alph_ptr->xyzCoord, beta_ptr->xyzCoord, 
                                                alph_ptr->xyzDisplacement, beta_ptr->xyzDisplacement ), 3.513460725771494e-08);
}

TEST_F(testCalculateSegStretchEnergy, testOutputValueNodes41)
{
    alph_ptr = &(box_ptr->masterNodeList[4]);
    beta_ptr = &(box_ptr->masterNodeList[1]);

    alph_ptr->xyzDisplacement[0] = 0.01250598041564164;
    alph_ptr->xyzDisplacement[1] = -0.006248884007600541;
    alph_ptr->xyzDisplacement[2] = 0;

    beta_ptr->xyzDisplacement[0] = 0.05;
    beta_ptr->xyzDisplacement[1] = 0;
    beta_ptr->xyzDisplacement[2] = 0;

    EXPECT_DOUBLE_EQ(calculateSegStretchEnergy( box_ptr, par_ptr, fIndex, 
                                                alph_ptr->xyzCoord, beta_ptr->xyzCoord, 
                                                alph_ptr->xyzDisplacement, beta_ptr->xyzDisplacement ), 3.511354163195315e-08);
}


struct testCalculateSegBendEnergy : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    PetscInt fIndex;
    Node *alph_ptr;
    Node *omeg_ptr;
    Node *beta_ptr;
    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 0.05, 1.0);

        alph_ptr = &(box_ptr->masterNodeList[0]);
        omeg_ptr = &(box_ptr->masterNodeList[4]);
        beta_ptr = &(box_ptr->masterNodeList[1]);
        /*
        alpha_ptr->xyzDisplacement[0] = 0;
        alpha_ptr->xyzDisplacement[1] = 0;
        alpha_ptr->xyzDisplacement[2] = 0;
        */
        omeg_ptr->xyzDisplacement[0] = 0.01250598041564164;
        omeg_ptr->xyzDisplacement[1] = -0.006248884007600541;
        omeg_ptr->xyzDisplacement[2] = 0;

        beta_ptr->xyzDisplacement[0] = 0.05;
        beta_ptr->xyzDisplacement[1] = 0;
        beta_ptr->xyzDisplacement[2] = 0;

        fIndex = 0;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
    }
};


TEST_F(testCalculateSegBendEnergy, testOutputValueNodes041)
{
    EXPECT_DOUBLE_EQ(calculateSegBendEnergy( box_ptr, par_ptr, fIndex, 
                                                alph_ptr->xyzCoord, 
                                                omeg_ptr->xyzCoord, 
                                                beta_ptr->xyzCoord,
                                                alph_ptr->xyzDisplacement, 
                                                omeg_ptr->xyzDisplacement, 
                                                beta_ptr->xyzDisplacement ), 1.75473419406617e-11);
}


struct testCalculateFibreStretchEnergy : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    PetscInt fIndex;
    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 0.05, 1.0);

        box_ptr->masterNodeList[1].xyzDisplacement[0] = 0.05;
        box_ptr->masterNodeList[4].xyzDisplacement[0] = 0.01250598041564164;
        box_ptr->masterNodeList[4].xyzDisplacement[1] = -0.006248884007600541;
        box_ptr->masterNodeList[4].xyzDisplacement[2] = 0;

        fIndex = 0;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
    }
};


TEST_F(testCalculateFibreStretchEnergy, testErrorOutput)
{
    EXPECT_EQ(calculateFibreStretchEnergy(box_ptr, par_ptr, fIndex), 0);
}


TEST_F(testCalculateFibreStretchEnergy, testOutputValueNodes041)
{
    calculateFibreStretchEnergy(box_ptr, par_ptr, fIndex);
    EXPECT_DOUBLE_EQ(box_ptr->masterFibreList[0].fibreStreEnergy, 7.024814888966809e-08);
}


struct testCalculateFibreBendEnergy : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    PetscInt fIndex;
    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 0.05, 1.0);

        box_ptr->masterNodeList[1].xyzDisplacement[0] = 0.05;
        box_ptr->masterNodeList[4].xyzDisplacement[0] = 0.01250598041564164;
        box_ptr->masterNodeList[4].xyzDisplacement[1] = -0.006248884007600541;
        box_ptr->masterNodeList[4].xyzDisplacement[2] = 0;

        fIndex = 0;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
    }
};


TEST_F(testCalculateFibreBendEnergy, testErrorOutput)
{
    EXPECT_DOUBLE_EQ(calculateFibreBendEnergy(box_ptr, par_ptr, fIndex), 0);
}


TEST_F(testCalculateFibreBendEnergy, testOutputValueNodes041)
{
    calculateFibreBendEnergy(box_ptr, par_ptr, fIndex);
    EXPECT_DOUBLE_EQ(box_ptr->masterFibreList[0].fibreBendEnergy, 1.75473419406617e-11);
}


struct testCalculateEnergy : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 0.05, 1.0);

        box_ptr->masterNodeList[1].xyzDisplacement[0] = 0.05;
        box_ptr->masterNodeList[4].xyzDisplacement[0] = 0.01250598041564164;
        box_ptr->masterNodeList[4].xyzDisplacement[1] = -0.006248884007600541;
        box_ptr->masterNodeList[4].xyzDisplacement[2] = 0;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
    }
};


TEST_F(testCalculateEnergy, testErrorOutput)
{
    EXPECT_EQ(calculateEnergy(box_ptr, par_ptr), 0);
}


TEST_F(testCalculateEnergy, testCalculatedValues)
{
    calculateEnergy(box_ptr, par_ptr);

    EXPECT_DOUBLE_EQ(par_ptr->energyStre, 7.024815465631318e-08);
    EXPECT_DOUBLE_EQ(par_ptr->energyBend, 1.75473419406617e-11);
    EXPECT_DOUBLE_EQ(par_ptr->energyTotl, 7.026570199825385e-08);
}


struct testCalculateVolume : ::testing::Test
{
    Box *box_ptr;
    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testCalculateVolume, testErrorOutput)
{
    EXPECT_DOUBLE_EQ(calculateVolume(box_ptr), 0.25);
}


struct testCalculateAperiodicRange : ::testing::Test
{
    Box *box_ptr;
    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testCalculateAperiodicRange, testCalculatedXRange)
{
    EXPECT_DOUBLE_EQ(calculateAperiodicRange(box_ptr, 0), 0.5);
}


TEST_F(testCalculateAperiodicRange, testCalculatedYRange)
{
    EXPECT_DOUBLE_EQ(calculateAperiodicRange(box_ptr, 1), 1.0);
}


TEST_F(testCalculateAperiodicRange, testCalculatedZRange)
{
    EXPECT_DOUBLE_EQ(calculateAperiodicRange(box_ptr, 2), 0.5);
}


struct testCalculateShearModulus : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 0.05, 1.0);

        box_ptr->masterNodeList[1].xyzDisplacement[0] = 0.05;
        box_ptr->masterNodeList[4].xyzDisplacement[0] = 0.01250598041564164;
        box_ptr->masterNodeList[4].xyzDisplacement[1] = -0.006248884007600541;
        box_ptr->masterNodeList[4].xyzDisplacement[2] = 0;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
    }
};


TEST_F(testCalculateShearModulus, testErrorOutput)
{
    EXPECT_EQ(calculateShearModulus(box_ptr, par_ptr), 0);
}


TEST_F(testCalculateShearModulus, testCalculatedValues)
{
    calculateShearModulus(box_ptr, par_ptr);

    EXPECT_DOUBLE_EQ(par_ptr->energyStre, 7.024815465631318e-08);
    EXPECT_DOUBLE_EQ(par_ptr->energyBend, 1.75473419406617e-11);
    EXPECT_DOUBLE_EQ(par_ptr->energyTotl, 7.026570199825385e-08);

    EXPECT_DOUBLE_EQ(par_ptr->shearModulus, 0.0002248502463944123);
}

} /* namespace */
