#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "vectorOperations.h"
}


namespace {

TEST(testMinInt, testOutputValues)
{
	EXPECT_TRUE(minInt( 1, 2) ==  1);
	EXPECT_TRUE(minInt( 4, 4) ==  4);
	EXPECT_TRUE(minInt( 7, 3) ==  3);
	EXPECT_TRUE(minInt( 1,-2) == -2);
	EXPECT_TRUE(minInt(-4, 4) == -4);
	EXPECT_TRUE(minInt( 7,-3) == -3);
}


TEST(testMaxInt, testOutputValues)
{
	EXPECT_TRUE(maxInt( 1, 2) ==  2);
	EXPECT_TRUE(maxInt( 4, 4) ==  4);
	EXPECT_TRUE(maxInt( 7, 3) ==  7);
	EXPECT_TRUE(maxInt( 1,-2) ==  1);
	EXPECT_TRUE(maxInt(-4, 4) ==  4);
	EXPECT_TRUE(maxInt( 7,-3) ==  7);
	EXPECT_TRUE(maxInt(-4,-3) == -3);
}


TEST(testMinScalar, testOutputValues)
{
	EXPECT_TRUE(minScalar( 1.0, 2.0) ==  1.0);
	EXPECT_TRUE(minScalar( 4.0, 4.0) ==  4.0);
	EXPECT_TRUE(minScalar( 7.0, 3.0) ==  3.0);
	EXPECT_TRUE(minScalar( 1.0,-2.0) == -2.0);
	EXPECT_TRUE(minScalar(-4.0, 4.0) == -4.0);
	EXPECT_TRUE(minScalar( 7.0,-3.0) == -3.0);
}


TEST(testMaxScalar, testOutputValues)
{
	EXPECT_TRUE(maxScalar( 1.8, 2.2) ==  2.2);
	EXPECT_TRUE(maxScalar( 4.2, 4.2) ==  4.2);
	EXPECT_TRUE(maxScalar( 7.8, 3.2) ==  7.8);
	EXPECT_TRUE(maxScalar( 1.8,-2.2) ==  1.8);
	EXPECT_TRUE(maxScalar(-4.8, 4.2) ==  4.2);
	EXPECT_TRUE(maxScalar( 7.8,-3.2) ==  7.8);
	EXPECT_TRUE(maxScalar(-4.8,-3.2) == -3.2);
}


TEST(testVecDotProduct, testOutputValuesFor2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	PetscScalar vec1[2] = {1.0, 2.0};
	PetscScalar vec2[2] = {3.0, 4.0};

	EXPECT_TRUE(vecDotProduct(vec1, vec2) == 11);
}

TEST(testVecDotProduct, testOutputValuesFor3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	PetscScalar vec3[3] = {1.0, 2.0, 3.0};
	PetscScalar vec4[3] = {4.0, 5.0, 6.0};

	EXPECT_TRUE(vecDotProduct(vec3, vec4) == 32);
}


TEST(testVec2DCrossProduct, testOutputValues)
{
	if (DIMENSION != 2) GTEST_SKIP();

	PetscScalar vec1[3];
	PetscScalar vec2[2] = {1.0, 2.0};
	PetscScalar vec3[2] = {3.0, 4.0};

	EXPECT_TRUE(vec2DCrossProduct(vec1, vec2, vec3) == 0);

	EXPECT_DOUBLE_EQ(vec1[0],  0.0);
	EXPECT_DOUBLE_EQ(vec1[1],  0.0);
	EXPECT_DOUBLE_EQ(vec1[2], -2.0);
}


TEST(testVec3DCrossProduct, testOutputValues)
{
	if (DIMENSION != 3) GTEST_SKIP();

	PetscScalar vec1[3];
	PetscScalar vec2[3]  = {1.0, 2.0, 3.0};
	PetscScalar vec3[3]  = {4.0, 5.0, 6.0};

	EXPECT_TRUE(vec3DCrossProduct(vec1, vec2, vec3) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], -3.0);
	EXPECT_DOUBLE_EQ(vec1[1],  6.0);
	EXPECT_DOUBLE_EQ(vec1[2], -3.0);
}


TEST(testVecMagnitude, testOutputValuesFor2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	PetscScalar vec1[2] = {1.0, 2.0};

	EXPECT_DOUBLE_EQ(vecMagnitude(vec1), sqrt(5));
}


TEST(testVecMagnitude, testOutputValuesFor3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	PetscScalar vec2[3] = {4.0, 2.0, 3.0};

	EXPECT_DOUBLE_EQ(vecMagnitude(vec2), sqrt(29));
}


TEST(testVecAddition, testOutputValuesFor2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 10;
	box_ptr->xyzDimension[1] 	= 10;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;

	PetscScalar vec1[2];
	PetscScalar vec2[2]  		= {3.0, 4.0};
	PetscScalar vec3[2]  		= {1.0, 1.0};

	EXPECT_TRUE(vecAddition(vec1, vec2, vec3, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec1[0],  4.0);
	EXPECT_DOUBLE_EQ(vec1[1],  5.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testVecAddition, testOutputValuesFor3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 10;
	box_ptr->xyzDimension[1] 	= 10;
	box_ptr->xyzDimension[2] 	= 10;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 1;

	PetscScalar vec4[3];
	PetscScalar vec5[3]  		= {1.0, 2.0, 3.0};
	PetscScalar vec6[3]  		= {4.0, 5.0, 6.0};

	EXPECT_TRUE(vecAddition(vec4, vec5, vec6, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec4[0],  5.0);
	EXPECT_DOUBLE_EQ(vec4[1],  7.0);
	EXPECT_DOUBLE_EQ(vec4[2], -1.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testMakeDistanceVec, testOutputValuesForMixedPeriodicityIn2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 10;
	box_ptr->xyzDimension[1] 	= 10;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;

	PetscScalar vec1[2];
	PetscScalar vec2[2]  		= {3.0, 4.0};
	PetscScalar vec3[2]  		= {1.0, 1.0};

	EXPECT_TRUE(makeDistanceVec(vec1, vec2, vec3, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], -2.0);
	EXPECT_DOUBLE_EQ(vec1[1], -3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testMakeDistanceVec, testOutputValuesForMixedPeriodicityIn3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 10;
	box_ptr->xyzDimension[1] 	= 10;
	box_ptr->xyzDimension[2] 	= 10;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 1;

	PetscScalar vec4[3];
	PetscScalar vec5[3]  		= {1.0, 2.0, 3.0};
	PetscScalar vec6[3]  		= {7.0, 5.0, 6.0};

	EXPECT_TRUE(makeDistanceVec(vec4, vec5, vec6, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec4[0], -4.0);
	EXPECT_DOUBLE_EQ(vec4[1],  3.0);
	EXPECT_DOUBLE_EQ(vec4[2],  3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testMakeDistanceVec, testOutputValuesForPeriodicIn2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 5;
	box_ptr->xyzDimension[1] 	= 5;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 1;

	PetscScalar vec1[2];
	PetscScalar vec2[2]  		= {3.0, 4.0};
	PetscScalar vec3[2]  		= {1.0, 1.0};

	EXPECT_TRUE(makeDistanceVec(vec1, vec2, vec3, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], -2.0);
	EXPECT_DOUBLE_EQ(vec1[1],  2.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testMakeDistanceVec, testOutputValuesForPeriodicIn3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 5;
	box_ptr->xyzDimension[1] 	= 5;
	box_ptr->xyzDimension[2] 	= 5;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 1;
	box_ptr->xyzPeriodic[2] 	= 1;

	PetscScalar vec4[3];
	PetscScalar vec5[3]  		= {1.0, 2.0, 3.0};
	PetscScalar vec6[3]  		= {4.0, 5.0, 6.0};

	EXPECT_TRUE(makeDistanceVec(vec4, vec5, vec6, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec4[0], -2.0);
	EXPECT_DOUBLE_EQ(vec4[1], -2.0);
	EXPECT_DOUBLE_EQ(vec4[2], -2.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testMakeDistanceVec, testOutputValuesForAperiodicIn2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 5;
	box_ptr->xyzDimension[1] 	= 5;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;

	PetscScalar vec1[2];
	PetscScalar vec2[2]  		= {3.0, 4.0};
	PetscScalar vec3[2]  		= {1.0, 1.0};

	EXPECT_TRUE(makeDistanceVec(vec1, vec2, vec3, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], -2.0);
	EXPECT_DOUBLE_EQ(vec1[1], -3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testMakeDistanceVec, testOutputValuesForAperiodicIn3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 5;
	box_ptr->xyzDimension[1] 	= 5;
	box_ptr->xyzDimension[2] 	= 5;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;

	PetscScalar vec4[3];
	PetscScalar vec5[3]  		= {1.0, 2.0, 3.0};
	PetscScalar vec6[3]  		= {4.0, 5.0, 6.0};

	EXPECT_TRUE(makeDistanceVec(vec4, vec5, vec6, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec4[0],  3.0);
	EXPECT_DOUBLE_EQ(vec4[1],  3.0);
	EXPECT_DOUBLE_EQ(vec4[2],  3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testMakeTangentVec, testOutputValuesFor2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	PetscScalar vec1[2];
	PetscScalar vec2[2]  = {3.0, 4.0};
	PetscScalar vec3[2]  = {1.0, 1.0};

	EXPECT_TRUE(makeTangentVec(vec1, vec2) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 3.0/5.0);
	EXPECT_DOUBLE_EQ(vec1[1], 4.0/5.0);

	EXPECT_TRUE(makeTangentVec(vec1, vec3) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 1.0/sqrt(2.0));
	EXPECT_DOUBLE_EQ(vec1[1], 1.0/sqrt(2.0));
}


TEST(testMakeTangentVec, testOutputValuesFor3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	PetscScalar vec4[3];
	PetscScalar vec5[3]  = {1.0, 2.0, 3.0};
	PetscScalar vec6[3]  = {4.0, 5.0, 6.0};

	EXPECT_TRUE(makeTangentVec(vec4, vec5) == 0);

	EXPECT_DOUBLE_EQ(vec4[0], 1.0/sqrt(14.0));
	EXPECT_DOUBLE_EQ(vec4[1], 2.0/sqrt(14.0));
	EXPECT_DOUBLE_EQ(vec4[2], 3.0/sqrt(14.0));

	EXPECT_TRUE(makeTangentVec(vec4, vec6) == 0);

	EXPECT_DOUBLE_EQ(vec4[0], 4.0/sqrt(77.0));
	EXPECT_DOUBLE_EQ(vec4[1], 5.0/sqrt(77.0));
	EXPECT_DOUBLE_EQ(vec4[2], 6.0/sqrt(77.0));
}


TEST(testMakePositionVec, testOutputValuesFor2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	PetscScalar vec1[2];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzCoord[0] 		 =  4.9;
	node_ptr->xyzCoord[1] 		 =  3.8;

	EXPECT_TRUE(makePositionVec(vec1, node_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 4.9);
	EXPECT_DOUBLE_EQ(vec1[1], 3.8);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


TEST(testMakePositionVec, testOutputValuesFor3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	PetscScalar vec2[3];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzCoord[0] 		 =  0.0;
	node_ptr->xyzCoord[1] 		 =  1.3;
	node_ptr->xyzCoord[2] 		 =  7.2;

	EXPECT_TRUE(makePositionVec(vec2, node_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec2[0], 0.0);
	EXPECT_DOUBLE_EQ(vec2[1], 1.3);
	EXPECT_DOUBLE_EQ(vec2[2], 7.2);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


TEST(testMakeDisplacementVec, testOutputValuesFor2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	PetscScalar vec1[2];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzDisplacement[0] =  3.7;
	node_ptr->xyzDisplacement[1] = -2.9;

	EXPECT_TRUE(makeDisplacementVec(vec1, node_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec1[0],  3.7);
	EXPECT_DOUBLE_EQ(vec1[1], -2.9);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


TEST(testMakeDisplacementVec, testOutputValuesFor3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	PetscScalar vec2[3];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzDisplacement[0] =  4.0;
	node_ptr->xyzDisplacement[1] =  0.3;
	node_ptr->xyzDisplacement[2] = -7.2;

	EXPECT_TRUE(makeDisplacementVec(vec2, node_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec2[0], 4.0);
	EXPECT_DOUBLE_EQ(vec2[1], 0.3);
	EXPECT_DOUBLE_EQ(vec2[2],-7.2);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


TEST(testUpdatePositionVec, testOutputValuesFor2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	PetscScalar vec1[2];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzCoord[0] 		 =  3.5;
	node_ptr->xyzCoord[1] 		 =  5.0;
	node_ptr->xyzDisplacement[0] =  1.3;
	node_ptr->xyzDisplacement[1] = -2.1;

	EXPECT_TRUE(updatePositionVec(vec1, node_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 4.8);
	EXPECT_DOUBLE_EQ(vec1[1], 2.9);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


TEST(testUpdatePositionVec, testOutputValuesFor3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	PetscScalar vec2[3];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzCoord[0] 		 =  0.0;
	node_ptr->xyzCoord[1] 		 =  4.3;
	node_ptr->xyzCoord[2] 		 =  9.2;
	node_ptr->xyzDisplacement[0] = -0.7;
	node_ptr->xyzDisplacement[1] =  0.0;
	node_ptr->xyzDisplacement[2] =  3.3;

	EXPECT_TRUE(updatePositionVec(vec2, node_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec2[0], -0.7);
	EXPECT_DOUBLE_EQ(vec2[1],  4.3);
	EXPECT_DOUBLE_EQ(vec2[2], 12.5);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


TEST(testNearestSegmentCopyDirN, testOutputValuesForPeriodicXIn2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopyDirN(vec1, box_ptr, 0) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 0.0);
	EXPECT_DOUBLE_EQ(vec1[1], 3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testNearestSegmentCopyDirN, testOutputValuesForPeriodicXIn3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopyDirN(vec2, box_ptr, 0) == 0);

	EXPECT_DOUBLE_EQ(vec2[0], 0.0);
	EXPECT_DOUBLE_EQ(vec2[1], 2.0);
	EXPECT_DOUBLE_EQ(vec2[2], 3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testNearestSegmentCopyDirN, testOutputValuesForAperiodicXIn2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopyDirN(vec1, box_ptr, 0) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 2.0);
	EXPECT_DOUBLE_EQ(vec1[1], 3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testNearestSegmentCopyDirN, testOutputValuesForAperiodicXIn3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopyDirN(vec2, box_ptr, 0) == 0);

	EXPECT_DOUBLE_EQ(vec2[0], 1.0);
	EXPECT_DOUBLE_EQ(vec2[1], 2.0);
	EXPECT_DOUBLE_EQ(vec2[2], 3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testNearestSegmentCopyDirN, testOutputValuesForPeriodicYIn2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 1;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopyDirN(vec1, box_ptr, 1) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 2.0);
	EXPECT_DOUBLE_EQ(vec1[1], 0.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testNearestSegmentCopyDirN, testOutputValuesForPeriodicYIn3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 1;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopyDirN(vec2, box_ptr, 1) == 0);

	EXPECT_DOUBLE_EQ(vec2[0], 1.0);
	EXPECT_DOUBLE_EQ(vec2[1], 0.0);
	EXPECT_DOUBLE_EQ(vec2[2], 3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testNearestSegmentCopyDirN, testOutputValuesForAperiodicYIn2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopyDirN(vec1, box_ptr, 1) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 2.0);
	EXPECT_DOUBLE_EQ(vec1[1], 3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testNearestSegmentCopyDirN, testOutputValuesForAperiodicYIn3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopyDirN(vec2, box_ptr, 1) == 0);

	EXPECT_DOUBLE_EQ(vec2[0], 1.0);
	EXPECT_DOUBLE_EQ(vec2[1], 2.0);
	EXPECT_DOUBLE_EQ(vec2[2], 3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testNearestSegmentCopyDirN, testOutputValuesForPeriodicZ)
{
	if (DIMENSION != 3) GTEST_SKIP();

	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 1;
	PetscScalar vec1[3] 		= {1.0, 2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopyDirN(vec1, box_ptr, 2) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 1.0);
	EXPECT_DOUBLE_EQ(vec1[1], 2.0);
	EXPECT_DOUBLE_EQ(vec1[2], 0.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


TEST(testNearestSegmentCopyDirN, testOutputValuesForAperiodicZ)
{
	if (DIMENSION != 3) GTEST_SKIP();

	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec1[3] 		= {1.0, 2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopyDirN(vec1, box_ptr, 2) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 1.0);
	EXPECT_DOUBLE_EQ(vec1[1], 2.0);
	EXPECT_DOUBLE_EQ(vec1[2], 3.0);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


struct testNearestSegmentCopy : ::testing::Test
{
	Box *box_ptr;
    void SetUp()
    {
        box_ptr 					= (Box *)malloc(sizeof(Box));
		box_ptr->xyzDimension[0] 	= 1;
		box_ptr->xyzDimension[1] 	= 1;
		box_ptr->xyzDimension[2] 	= 1;
    }

    void TearDown()
    {
        /* clean up */
    	free(box_ptr); box_ptr = NULL;
    }
};


TEST_F(testNearestSegmentCopy, testOutputValuesForPeriodicIn2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 1;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopy(vec1, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 0.0);
	EXPECT_DOUBLE_EQ(vec1[1], 0.0);
}


TEST_F(testNearestSegmentCopy, testOutputValuesForPeriodicIn3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 1;
	box_ptr->xyzPeriodic[2] 	= 1;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopy(vec2, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec2[0], 0.0);
	EXPECT_DOUBLE_EQ(vec2[1], 0.0);
	EXPECT_DOUBLE_EQ(vec2[2], 0.0);
}


TEST_F(testNearestSegmentCopy, testOutputValuesForAPeriodicIn2D)
{
	if (DIMENSION != 2) GTEST_SKIP();

	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopy(vec1, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec1[0], 2.0);
	EXPECT_DOUBLE_EQ(vec1[1], 3.0);
}


TEST_F(testNearestSegmentCopy, testOutputValuesForAPeriodicIn3D)
{
	if (DIMENSION != 3) GTEST_SKIP();

	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	EXPECT_TRUE(nearestSegmentCopy(vec2, box_ptr) == 0);

	EXPECT_DOUBLE_EQ(vec2[0], 1.0);
	EXPECT_DOUBLE_EQ(vec2[1], 2.0);
	EXPECT_DOUBLE_EQ(vec2[2], 3.0);
}

} /* namespace */