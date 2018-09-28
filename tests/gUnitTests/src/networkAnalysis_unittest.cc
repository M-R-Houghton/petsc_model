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
    EXPECT_DOUBLE_EQ( calculateSegStretchEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
                                        Node *alph_ptr, Node *beta_ptr ), 0.0);
}

TEST_F(testNetworkAnalysis, testErrorOutput)
{
    calculateSegBendEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
                                     Node *alph_ptr, Node *omeg_ptr, Node *beta_ptr );
}

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
    calculateStretchEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);
}


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
    calculateBendEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);
}


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
    calculateEnergy(Box *box_ptr, Parameters *par_ptr, PetscScalar *energy_ptr);
}


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
    calculateArea(Box *box_ptr);
}


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
    calculateVolume(Box *box_ptr);
}


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
    aperiodicRange(Box *box_ptr, PetscInt i);
}


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
    calculateShearModulus(Box *box_ptr, Parameters *par_ptr, PetscScalar *energy_ptr);
}


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
    calculateAffShearModulus(Box *box_ptr, Parameters *par_ptr, PetscScalar *energy_ptr);
}

} /* namespace */
