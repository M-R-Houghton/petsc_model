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
    void SetUp()
    {

    }

    void TearDown()
    {

    }
};


TEST_F(testNetworkAnalysis, testErrorOutput)
{
    EXPECT_EQ(networkAnalysis(), 0);
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


struct testCalculateStretchEnergy : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    PetscInt fIndex;
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


TEST_F(testCalculateStretchEnergy, testErrorOutput)
{
    EXPECT_DOUBLE_EQ(calculateStretchEnergy(box_ptr, par_ptr, fIndex), 0.0);
}


struct testCalculateBendEnergy : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    PetscInt fIndex;
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


TEST_F(testCalculateBendEnergy, testErrorOutput)
{
    EXPECT_DOUBLE_EQ(calculateBendEnergy(box_ptr, par_ptr, fIndex), 0.0);
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


struct testCalculateArea : ::testing::Test
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


TEST_F(testCalculateArea, testErrorOutput)
{
    EXPECT_DOUBLE_EQ(calculateArea(box_ptr), 0.0);
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
    EXPECT_DOUBLE_EQ(calculateVolume(box_ptr), 0.0);
}


struct testAperiodicRange : ::testing::Test
{
    Box *box_ptr;
    PetscInt i;
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


TEST_F(testAperiodicRange, testErrorOutput)
{
    EXPECT_DOUBLE_EQ(aperiodicRange(box_ptr, i), 0.0);
}


struct testCalculateShearModulus : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
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


TEST_F(testCalculateShearModulus, testErrorOutput)
{
    EXPECT_DOUBLE_EQ(calculateShearModulus(box_ptr, par_ptr), 0.0);
}

} /* namespace */
