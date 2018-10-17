#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "systemAssembly.h"
}


namespace {

struct testParMetis : ::testing::Test
{
    PetscMPIInt     size;
    PetscInt mlocal[2],n;

    void SetUp()
    {
        MPI_Comm_size(PETSC_COMM_WORLD,&size);

        /*
         Proc_0: mlocal=3,n=6,ja={3,4|4,5|3,4,5},ia={0,2,4,7}
         Proc_1: mlocal=3,n=6,ja={0,2,4|0,1,2,3,5|1,2,4},ia={0,3,8,11}
         */

        int i;
        for (i = 0; i < 2; i++) { mlocal[i] = 3; }
    }

    void TearDown()
    {

    }
};


TEST_F(testParMetis, testForMoreThanOneProcessor)
{
    if (size < 2) GTEST_SKIP();

    EXPECT_EQ(size, 2);
}


TEST_F(testParMetis, testMetisObjectValues)
{
    if (size < 2) GTEST_SKIP();

    EXPECT_EQ(mlocal[0], 3);

    /*
    Mat *Adj;
    MatPartitioning *part;
    IS *is, *isg;

    MatCreateMPIAdj(PETSC_COMM_WORLD, mlocal, n, ia, ja, Adj);

    MatPartitioningCreate(PETSC_COMM_WORLD, part);
    MatPartitioningSetAdjacency(part, Adj);
    MatPartitioningSetFromOptions(part);
    MatPartitioningApply(part, is);
    MatPartitioningDestroy(part);

    MatDestroy(Adj);
    ISPartitioningToNumbering(is, isg);

     */
}


struct testSystemAssembly : ::testing::Test
{
    PetscMPIInt     size;
    Box             *box_ptr;
    Parameters      *par_ptr;
    Mat             glbMat;
    Vec             glbVec;
    PetscScalar     locMat[6][6], locVec[6];
    Node            *alpha_ptr, *beta_ptr;
    PetscInt        N;

    void SetUp()
    {
        MPI_Comm_size(PETSC_COMM_WORLD,&size);
        MatCreate(PETSC_COMM_WORLD,&glbMat);
        MatSetFromOptions(glbMat);
        MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,6,6);
        MatSetUp(glbMat);

        VecCreate(PETSC_COMM_WORLD,&glbVec);
        VecSetFromOptions(glbVec);
        VecSetSizes(glbVec,PETSC_DECIDE,6);

        locMat[0][0] = 0;
        locVec[0] = 0;

        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 1.0, 1.0);

        alpha_ptr = &(box_ptr->masterNodeList[0]);
        beta_ptr = &(box_ptr->masterNodeList[1]);

        N = 2;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
        MatDestroy(&glbMat);
        VecDestroy(&glbVec);
    }
};


TEST_F(testSystemAssembly, testErrorOutput)
{
    if (size != 1) GTEST_SKIP();

    EXPECT_EQ(systemAssembly(box_ptr, par_ptr, glbMat, glbVec), 0);
}


TEST_F(testSystemAssembly, DISABLED_testGlobalMatValues)
{
    /* write a test for lmbAdd1 matrix value validation */
    /*
    0   0   0.0004443160618341999 
    0   2   -0.0002221163788895548
    0   1   -0.0002221774685299544  
    0   3   0.0002221108252858821

    2   0   -0.0002221163788895548
    2   2   0.0004443160618341999  
    2   1   0.0002221108252858821 
    2   3   -0.0002221774685299544 

    1   1   0.0006664842743580332 
    1   3   -0.0002220923132736397 
    1   0   -0.0002221774685299544 
    1   2   0.0002221108252858821 

    3   1   -0.0002220923132736397 
    3   3   0.0006664842743580332 
    3   0   0.0002221108252858821 
    3   2   -0.0002221774685299544 
    */
}


TEST_F(testSystemAssembly, DISABLED_testGlobalRHSVecValues)
{
    /* write a test for lmbAdd1 vector value validation */
    /*
    -1.110720734539591e-09 
     4.443105082305272e-05 
    -1.110720734539591e-09 
     2.221441469078285e-09 
    */
}

TEST(testReadInt, testRowFileReadIn) 
{
    PetscInt array[9];
    char fileName[100] = "../../data/row/idxm.f3tTripod1";
    readInt(fileName, array, 9);

    EXPECT_EQ(array[0], 0);
    EXPECT_EQ(array[1], 0);
    EXPECT_EQ(array[2], 0);

    EXPECT_EQ(array[3], 1);
    EXPECT_EQ(array[4], 1);
    EXPECT_EQ(array[5], 1);

    EXPECT_EQ(array[6], 2);
    EXPECT_EQ(array[7], 2);
    EXPECT_EQ(array[8], 2);
}

TEST(testReadInt, testColFileReadIn) 
{
    PetscInt array[9];
    char fileName[100] = "../../data/col/col.f3tTripod1";
    readInt(fileName, array, 9);

    EXPECT_EQ(array[0], 0);
    EXPECT_EQ(array[1], 1);
    EXPECT_EQ(array[2], 2);

    EXPECT_EQ(array[3], 0);
    EXPECT_EQ(array[4], 1);
    EXPECT_EQ(array[5], 2);

    EXPECT_EQ(array[6], 0);
    EXPECT_EQ(array[7], 1);
    EXPECT_EQ(array[8], 2);
}

TEST(testReadDbl, testMatFileReadIn) 
{
    PetscScalar array[9];
    char fileName[100] = "../../data/mat/mat.f3tTripod1";
    readDbl(fileName, array, 9);

    EXPECT_DOUBLE_EQ(array[0],  0.0003959445836206);
    EXPECT_DOUBLE_EQ(array[1], -0.0001075029206406);
    EXPECT_DOUBLE_EQ(array[2],  0.0000000000000000);

    EXPECT_DOUBLE_EQ(array[3], -0.0001075029206406);
    EXPECT_DOUBLE_EQ(array[4],  0.0015832388287110);
    EXPECT_DOUBLE_EQ(array[5],  0.0000000000000000);

    EXPECT_DOUBLE_EQ(array[6],  0.0000000000000000);
    EXPECT_DOUBLE_EQ(array[7],  0.0000000000000000);
    EXPECT_DOUBLE_EQ(array[8],  0.0001711864792787);
}

TEST(testReadDbl, testRhsFileReadIn) 
{
    PetscScalar array[3];
    char fileName[100] = "../../data/rhs/rhs.f3tTripod1";
    readDbl(fileName, array, 3);

    EXPECT_DOUBLE_EQ(array[0],  0.0000056234484900);
    EXPECT_DOUBLE_EQ(array[1], -0.0000112379052171);
    EXPECT_DOUBLE_EQ(array[2],  0.0000000000000000);
}

TEST(testWriteDbl, testSolFileWriteOut) 
{
    PetscScalar array[3];
    char fileName[100] = "../../data/sol/sol.f3tTripod1";

    array[0] =  1.23456;
    array[1] =  7.80000;
    array[2] = -9.87654;

    writeDbl(fileName, array, 3);

    EXPECT_DOUBLE_EQ(array[0],  1.23456);
    EXPECT_DOUBLE_EQ(array[1],  7.80000);
    EXPECT_DOUBLE_EQ(array[2], -9.87654);
}

struct testSolveAssembledMatrix : ::testing::Test
{
    PetscErrorCode ierr;
    PetscMPIInt size;
    PetscInt n;
    char const *rowFile;
    char const *colFile;
    char const *matFile;
    char const *rhsFile;
    char const *solFile;

    void SetUp()
    {
        /* set up */
        MPI_Comm_size(PETSC_COMM_WORLD,&size);
        n       = 3;
        rowFile = "../../data/row/row.f3tTripod1";
        colFile = "../../data/col/col.f3tTripod1";
        matFile = "../../data/mat/mat.f3tTripod1";
        rhsFile = "../../data/rhs/rhs.f3tTripod1";
        solFile = "../../data/sol/sol.f3tTripod1";
    }

    void TearDown()
    {
        /* clean up */
    }
};

TEST_F(testSolveAssembledMatrix, testIfTripodCaseRuns)
{
    if (size != 1) GTEST_SKIP();

    ierr = solveAssembledMatrix(rowFile,colFile,matFile,rhsFile,solFile,n);
    EXPECT_EQ(ierr, 0);
}

TEST_F(testSolveAssembledMatrix, testTripodSolFileOutput)
{
    if (size != 1) GTEST_SKIP();

    PetscScalar array[n];

    solveAssembledMatrix(rowFile,colFile,matFile,rhsFile,solFile,n);

    readDbl(solFile, array, n);

    EXPECT_DOUBLE_EQ(array[0],  0.0125059804156416);
    EXPECT_DOUBLE_EQ(array[1], -0.0062488840076005);
    EXPECT_DOUBLE_EQ(array[2],  0.0000000000000000);
}


struct testLattice2Solve : ::testing::Test
{
    PetscErrorCode ierr;
    PetscMPIInt size;
    PetscInt n;
    char const *rowFile;
    char const *colFile;
    char const *matFile;
    char const *rhsFile;
    char const *solFile;

    void SetUp()
    {
        /* set up */
        MPI_Comm_size(PETSC_COMM_WORLD,&size);
        n       = 2355;
        rowFile = "../../data/row/row.f3tLat02";
        colFile = "../../data/col/col.f3tLat02";
        matFile = "../../data/mat/mat.f3tLat02";
        rhsFile = "../../data/rhs/rhs.f3tLat02";
        solFile = "../../data/sol/sol.f3tLat02";
    }

    void TearDown()
    {
        /* clean up */
    }
};

TEST_F(testLattice2Solve, testLatticeSolFileOutput)
{
    if (size != 1) GTEST_SKIP();

    PetscScalar array[n];

    solveAssembledMatrix(rowFile,colFile,matFile,rhsFile,solFile,n);

    readDbl(solFile, array, n);
    EXPECT_FLOAT_EQ(array[0],  0.0209387473297936);
    EXPECT_FLOAT_EQ(array[1],  0.0196075653537027);
    EXPECT_FLOAT_EQ(array[2],  0.0220867548145517);
    EXPECT_FLOAT_EQ(array[3],  0.0198649352083624);
    EXPECT_FLOAT_EQ(array[4],  0.0235944741704036);
    EXPECT_FLOAT_EQ(array[5],  0.0203577389170724);
    EXPECT_FLOAT_EQ(array[6],  0.0252535447741999);
    EXPECT_FLOAT_EQ(array[7],  0.0207797440129432);
    EXPECT_FLOAT_EQ(array[8],  0.0269823082382125);
    EXPECT_FLOAT_EQ(array[9],  0.0215428695902322);
}

} /* namespace */
