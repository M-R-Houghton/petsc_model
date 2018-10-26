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
    PetscMPIInt size, rank;

    void SetUp()
    {
        MPI_Comm_size(PETSC_COMM_WORLD,&size);
        MPI_Comm_rank(PETSC_COMM_WORLD,&rank);
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


TEST_F(testParMetis, testMetisSerialExample)
{
    if (size > 1) GTEST_SKIP();

    PetscInt mlocal=7,n=7;

    /* I think this needs to be dynamic allocation when partitioning */
    PetscInt *ia, *ja;
    PetscMalloc(8*sizeof(PetscInt), &ia);
    PetscMalloc(22*sizeof(PetscInt), &ja);


    /*
     Proc_0: mlocal=7,n=7,ja={3,4|4,5|3,4,5},ia={0,2,4,7}
     */

    ia[0] = 0, ia[1] = 3, ia[2] = 6, ia[3] = 10, ia[4] = 14, ia[5] = 17, ia[6] = 20, ia[7] = 22;

    ja[0]  = 1, ja[1]  = 2, ja[2]  = 4;
    ja[3]  = 0, ja[4]  = 2, ja[5]  = 3;
    ja[6]  = 0, ja[7]  = 1, ja[8]  = 3, ja[9]  = 4;
    ja[10] = 1, ja[11] = 2, ja[12] = 5, ja[13] = 6;
    ja[14] = 0, ja[15] = 2, ja[16] = 5;
    ja[17] = 3, ja[18] = 4, ja[19] = 6;
    ja[20] = 3, ja[21] = 5;

    /*
     * Running but don't think this is working yet
     */
    ///*
    Mat Adj;
    MatPartitioning part;
    IS is, isg;

    MatCreateMPIAdj(PETSC_COMM_WORLD, mlocal, n, ia, ja, NULL, &Adj);

    MatPartitioningCreate(PETSC_COMM_WORLD, &part);
    MatPartitioningSetAdjacency(part, Adj);
    MatPartitioningSetFromOptions(part);
    MatPartitioningApply(part, &is);
    MatPartitioningDestroy(&part);

    ISView(is,PETSC_VIEWER_STDOUT_WORLD);

    MatDestroy(&Adj);
    ISPartitioningToNumbering(is, &isg);

    FAIL();
     //*/
}


TEST_F(testParMetis, testMetisParallelExample)
{
    if (size != 2) GTEST_SKIP();

    PetscInt mlocal=3,n=6;

    /* I think this needs to be dynamic allocation when partitioning */
    PetscInt *ia, *ja;
    PetscMalloc(4*sizeof(PetscInt), &ia);
    PetscMalloc(11*sizeof(PetscInt), &ja);

    /*
     Proc_0: mlocal=3,n=6,ja={3,4|4,5|3,4,5},ia={0,2,4,7}
     Proc_1: mlocal=3,n=6,ja={0,2,4|0,1,2,3,5|1,2,4},ia={0,3,8,11}
     */

    if (!rank)
    {
        ia[0] = 0, ia[1] = 2, ia[2] = 4, ia[3] = 7;
        ja[0] = 3, ja[1] = 4, ja[2] = 4, ja[3] = 5, ja[4] = 3, ja[5] = 4, ja[6] = 5;
    }
    else
    {
        ia[0] = 0, ia[1] = 3, ia[2] = 8, ia[3] = 11;
        ja[0] = 0, ja[1] = 2, ja[2] = 4, ja[3] = 0, ja[4] = 1, ja[5] = 2, ja[6] = 3, ja[7] = 5, ja[8] = 1, ja[9] = 2, ja[10] = 4;
    }

    /*
     * Running but don't think this is working yet
     */
    ///*
    Mat Adj;
    MatPartitioning part;
    IS is, isg;

    MatCreateMPIAdj(PETSC_COMM_WORLD, mlocal, n, ia, ja, NULL, &Adj);

    MatPartitioningCreate(PETSC_COMM_WORLD, &part);
    MatPartitioningSetAdjacency(part, Adj);
    MatPartitioningSetFromOptions(part);
    MatPartitioningApply(part, &is);
    MatPartitioningDestroy(&part);

    ISView(is,PETSC_VIEWER_STDOUT_WORLD);

    MatDestroy(&Adj);
    ISPartitioningToNumbering(is, &isg);

    FAIL();
    //*/
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


struct testApplyElasticMediumToMatrix : ::testing::Test
{
    PetscMPIInt     size;
    Box             *box_ptr;
    Parameters      *par_ptr;
    Mat             glbMat;
    Vec             glbVec;
    PetscScalar     lambda;

    void SetUp()
    {
        MPI_Comm_size(PETSC_COMM_WORLD,&size);
        MatCreate(PETSC_COMM_WORLD,&glbMat);
        MatSetFromOptions(glbMat);
        MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,6,6);
        MatSetUp(glbMat);

        MatZeroEntries(glbMat);

        MatAssemblyBegin(glbMat,MAT_FINAL_ASSEMBLY);
        MatAssemblyEnd(glbMat,MAT_FINAL_ASSEMBLY);

        VecCreate(PETSC_COMM_WORLD,&glbVec);
        VecSetFromOptions(glbVec);
        VecSetSizes(glbVec,PETSC_DECIDE,6);

        lambda = -1e-5;
    }

    void TearDown()
    {
        MatDestroy(&glbMat);
    }
};


TEST_F(testApplyElasticMediumToMatrix, testErrorOutput)
{
    if (size != 1) GTEST_SKIP();

    EXPECT_EQ(applyElasticMediumToMatrix(glbMat, lambda), 0);
}


TEST_F(testApplyElasticMediumToMatrix, testDiagonalValues)
{
    if (size != 1) GTEST_SKIP();

    applyElasticMediumToMatrix(glbMat, lambda);

    PetscScalar *diagonal;
    MatGetDiagonal(glbMat,glbVec);

    VecGetArray(glbVec, &diagonal);
    EXPECT_DOUBLE_EQ(diagonal[0], -1e-5);
    EXPECT_DOUBLE_EQ(diagonal[1], -1e-5);
    EXPECT_DOUBLE_EQ(diagonal[2], -1e-5);
    VecRestoreArray(glbVec, &diagonal);
}


struct testApplyElasticMediumToRHSVector : ::testing::Test
{
    PetscMPIInt     size;
    Box             *box_ptr;
    Parameters      *par_ptr;
    Mat             glbMat;
    Vec             glbVec;
    PetscScalar     lambda;

    void SetUp()
    {
        MPI_Comm_size(PETSC_COMM_WORLD,&size);

        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        VecCreate(PETSC_COMM_WORLD,&glbVec);
        VecSetFromOptions(glbVec);
        VecSetSizes(glbVec,PETSC_DECIDE,6);

        lambda = -1e-5;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        VecDestroy(&glbVec);
    }
};


TEST_F(testApplyElasticMediumToRHSVector, testErrorOutput)
{
    if (size != 1) GTEST_SKIP();

    EXPECT_EQ(applyElasticMediumToRHSVector(box_ptr, glbVec, lambda), 0);
}


struct testApplyElasticMedium : ::testing::Test
{
    PetscMPIInt     size;
    Box             *box_ptr;
    Parameters      *par_ptr;
    Mat             glbMat;
    Vec             glbVec;
    PetscInt        N;

    void SetUp()
    {
        MPI_Comm_size(PETSC_COMM_WORLD,&size);
        MatCreate(PETSC_COMM_WORLD,&glbMat);
        MatSetFromOptions(glbMat);
        MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,6,6);
        MatSetUp(glbMat);

        MatZeroEntries(glbMat);

        MatAssemblyBegin(glbMat,MAT_FINAL_ASSEMBLY);
        MatAssemblyEnd(glbMat,MAT_FINAL_ASSEMBLY);

        VecCreate(PETSC_COMM_WORLD,&glbVec);
        VecSetFromOptions(glbVec);
        VecSetSizes(glbVec,PETSC_DECIDE,6);

        N = 2;
    }

    void TearDown()
    {
        MatDestroy(&glbMat);
    }
};


TEST_F(testApplyElasticMedium, testErrorOutput)
{
    if (size != 1) GTEST_SKIP();

    EXPECT_EQ(applyElasticMedium(glbMat), 0);
}


TEST_F(testApplyElasticMedium, testDiagonalValues)
{
    if (size != 1) GTEST_SKIP();

    applyElasticMedium(glbMat);

    PetscScalar *diagonal;
    MatGetDiagonal(glbMat,glbVec);

    VecGetArray(glbVec, &diagonal);
    EXPECT_DOUBLE_EQ(diagonal[0], -1e-5);
    EXPECT_DOUBLE_EQ(diagonal[1], -1e-5);
    EXPECT_DOUBLE_EQ(diagonal[2], -1e-5);
    VecRestoreArray(glbVec, &diagonal);
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
