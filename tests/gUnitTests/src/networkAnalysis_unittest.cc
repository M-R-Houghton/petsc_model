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

        par_ptr = makeParameters(fileToRead, fileToRead, 1.0, 1.0);
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

        par_ptr = makeParameters(fileToRead, fileToRead, 1.0, 1.0);

        alph_ptr = &(box_ptr->masterNodeList[0]);
        beta_ptr = &(box_ptr->masterNodeList[4]);

        fIndex = 0;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
    }
};


TEST_F(testCalculateSegStretchEnergy, testErrorOutput)
{
    EXPECT_DOUBLE_EQ(calculateSegStretchEnergy( box_ptr, par_ptr, fIndex, alph_ptr, beta_ptr ), 0.0);
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

        par_ptr = makeParameters(fileToRead, fileToRead, 1.0, 1.0);

        alph_ptr = &(box_ptr->masterNodeList[0]);
        omeg_ptr = &(box_ptr->masterNodeList[4]);
        beta_ptr = &(box_ptr->masterNodeList[1]);

        fIndex = 0;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
    }
};


TEST_F(testCalculateSegBendEnergy, testErrorOutput)
{
    EXPECT_DOUBLE_EQ(calculateSegBendEnergy( box_ptr, par_ptr, fIndex, alph_ptr, omeg_ptr, beta_ptr ), 0.0);
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

        par_ptr = makeParameters(fileToRead, fileToRead, 1.0, 1.0);

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
    EXPECT_DOUBLE_EQ(calculateFibreStretchEnergy(box_ptr, par_ptr, fIndex), 0.0);
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

        par_ptr = makeParameters(fileToRead, fileToRead, 1.0, 1.0);

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
    EXPECT_DOUBLE_EQ(calculateFibreBendEnergy(box_ptr, par_ptr, fIndex), 0.0);
}


struct testCalculateEnergy : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 1.0, 1.0);
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

    EXPECT_DOUBLE_EQ(par_ptr->energyStre, 0.0);
    EXPECT_DOUBLE_EQ(par_ptr->energyBend, 0.0);
    EXPECT_DOUBLE_EQ(par_ptr->energyTotl, 0.0);
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

        par_ptr = makeParameters(fileToRead, fileToRead, 1.0, 1.0);
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

    EXPECT_DOUBLE_EQ(par_ptr->energyStre, 0.0);
    EXPECT_DOUBLE_EQ(par_ptr->energyBend, 0.0);
    EXPECT_DOUBLE_EQ(par_ptr->energyTotl, 0.0);

    EXPECT_DOUBLE_EQ(par_ptr->shearModulus, 0.0);
}

} /* namespace */
