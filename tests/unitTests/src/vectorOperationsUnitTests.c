#include <criterion/criterion.h>
#include "vectorOperations.h"

Test(testMinInt, testOutputValues)
{
	cr_expect(minInt( 1, 2) ==  1);
	cr_expect(minInt( 4, 4) ==  4);
	cr_expect(minInt( 7, 3) ==  3);
	cr_expect(minInt( 1,-2) == -2);
	cr_expect(minInt(-4, 4) == -4);
	cr_expect(minInt( 7,-3) == -3);
}


Test(testMaxInt, testOutputValues)
{
	cr_expect(maxInt( 1, 2) ==  2);
	cr_expect(maxInt( 4, 4) ==  4);
	cr_expect(maxInt( 7, 3) ==  7);
	cr_expect(maxInt( 1,-2) ==  1);
	cr_expect(maxInt(-4, 4) ==  4);
	cr_expect(maxInt( 7,-3) ==  7);
	cr_expect(maxInt(-4,-3) == -3);
}


Test(testMinScalar, testOutputValues)
{
	cr_expect(minScalar( 1.0, 2.0) ==  1.0);
	cr_expect(minScalar( 4.0, 4.0) ==  4.0);
	cr_expect(minScalar( 7.0, 3.0) ==  3.0);
	cr_expect(minScalar( 1.0,-2.0) == -2.0);
	cr_expect(minScalar(-4.0, 4.0) == -4.0);
	cr_expect(minScalar( 7.0,-3.0) == -3.0);
}


Test(testMaxScalar, testOutputValues)
{
	cr_expect(maxScalar( 1.8, 2.2) ==  2.2);
	cr_expect(maxScalar( 4.2, 4.2) ==  4.2);
	cr_expect(maxScalar( 7.8, 3.2) ==  7.8);
	cr_expect(maxScalar( 1.8,-2.2) ==  1.8);
	cr_expect(maxScalar(-4.8, 4.2) ==  4.2);
	cr_expect(maxScalar( 7.8,-3.2) ==  7.8);
	cr_expect(maxScalar(-4.8,-3.2) == -3.2);
}


Test(testVecDotProduct, testOutputValuesFor2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar vec1[2] = {1.0, 2.0};
	PetscScalar vec2[2] = {3.0, 4.0};

	cr_expect(vecDotProduct(vec1, vec2) == 11);
}

Test(testVecDotProduct, testOutputValuesFor3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar vec3[3] = {1.0, 2.0, 3.0};
	PetscScalar vec4[3] = {4.0, 5.0, 6.0};

	cr_expect(vecDotProduct(vec3, vec4) == 32);
}


Test(testVec2DCrossProduct, testOutputValues)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar vec1[2] = {1.0, 2.0};
	PetscScalar vec2[2] = {3.0, 4.0};

	cr_expect(vec2DCrossProduct(vec1, vec2) == -2.0);
}


Test(testVec3DCrossProduct, testOutputValues)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 	 = 1e-12;
	PetscScalar vec1[3];
	PetscScalar vec2[3]  = {1.0, 2.0, 3.0};
	PetscScalar vec3[3]  = {4.0, 5.0, 6.0};

	cr_expect(vec3DCrossProduct(vec1, vec2, vec3) == 0);

	cr_expect_float_eq(vec1[0], -3.0, tol);
	cr_expect_float_eq(vec1[1],  6.0, tol);
	cr_expect_float_eq(vec1[2], -3.0, tol);
}


Test(testVecMagnitude, testOutputValuesFor2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 	= 1e-12;
	PetscScalar vec1[2] = {1.0, 2.0};

	cr_expect_float_eq(vecMagnitude(vec1), sqrt(5),  tol);
}


Test(testVecMagnitude, testOutputValuesFor3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 	= 1e-12;
	PetscScalar vec2[3] = {4.0, 2.0, 3.0};

	cr_expect_float_eq(vecMagnitude(vec2), sqrt(29), tol);
}


Test(testVecAddition, testOutputValuesFor2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 	 		= 1e-12;
	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 10;
	box_ptr->xyzDimension[1] 	= 10;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;

	PetscScalar vec1[2];
	PetscScalar vec2[2]  		= {3.0, 4.0};
	PetscScalar vec3[2]  		= {1.0, 1.0};

	cr_expect(vecAddition(vec1, vec2, vec3, box_ptr) == 0);

	cr_expect_float_eq(vec1[0],  4.0, tol);
	cr_expect_float_eq(vec1[1],  5.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testVecAddition, testOutputValuesFor3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 	 		= 1e-12;
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

	cr_expect(vecAddition(vec4, vec5, vec6, box_ptr) == 0);

	cr_expect_float_eq(vec4[0],  5.0, tol);
	cr_expect_float_eq(vec4[1],  7.0, tol);
	cr_expect_float_eq(vec4[2], -1.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testMakeDistanceVec, testOutputValuesForMixedPeriodicityIn2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 	 		= 1e-12;
	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 10;
	box_ptr->xyzDimension[1] 	= 10;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;

	PetscScalar vec1[2];
	PetscScalar vec2[2]  		= {3.0, 4.0};
	PetscScalar vec3[2]  		= {1.0, 1.0};

	cr_expect(makeDistanceVec(vec1, vec2, vec3, box_ptr) == 0);

	cr_expect_float_eq(vec1[0], -2.0, tol);
	cr_expect_float_eq(vec1[1], -3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testMakeDistanceVec, testOutputValuesForMixedPeriodicityIn3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 	 		= 1e-12;
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

	cr_expect(makeDistanceVec(vec4, vec5, vec6, box_ptr) == 0);

	cr_expect_float_eq(vec4[0], -4.0, tol);
	cr_expect_float_eq(vec4[1],  3.0, tol);
	cr_expect_float_eq(vec4[2],  3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testMakeDistanceVec, testOutputValuesForPeriodicIn2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 	 		= 1e-12;
	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 5;
	box_ptr->xyzDimension[1] 	= 5;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 1;

	PetscScalar vec1[2];
	PetscScalar vec2[2]  		= {3.0, 4.0};
	PetscScalar vec3[2]  		= {1.0, 1.0};

	cr_expect(makeDistanceVec(vec1, vec2, vec3, box_ptr) == 0);

	cr_expect_float_eq(vec1[0], -2.0, tol);
	cr_expect_float_eq(vec1[1],  2.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testMakeDistanceVec, testOutputValuesForPeriodicIn3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 	 		= 1e-12;
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

	cr_expect(makeDistanceVec(vec4, vec5, vec6, box_ptr) == 0);

	cr_expect_float_eq(vec4[0], -2.0, tol);
	cr_expect_float_eq(vec4[1], -2.0, tol);
	cr_expect_float_eq(vec4[2], -2.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testMakeDistanceVec, testOutputValuesForAperiodicIn2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 	 		= 1e-12;
	Box *box_ptr 		 		= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 5;
	box_ptr->xyzDimension[1] 	= 5;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;

	PetscScalar vec1[2];
	PetscScalar vec2[2]  		= {3.0, 4.0};
	PetscScalar vec3[2]  		= {1.0, 1.0};

	cr_expect(makeDistanceVec(vec1, vec2, vec3, box_ptr) == 0);

	cr_expect_float_eq(vec1[0], -2.0, tol);
	cr_expect_float_eq(vec1[1], -3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testMakeDistanceVec, testOutputValuesForAperiodicIn3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 	 		= 1e-12;
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

	cr_expect(makeDistanceVec(vec4, vec5, vec6, box_ptr) == 0);

	cr_expect_float_eq(vec4[0],  3.0, tol);
	cr_expect_float_eq(vec4[1],  3.0, tol);
	cr_expect_float_eq(vec4[2],  3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testMakeTangentVec, testOutputValuesFor2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 	 = 1e-12;
	PetscScalar vec1[2];
	PetscScalar vec2[2]  = {3.0, 4.0};
	PetscScalar vec3[2]  = {1.0, 1.0};

	cr_expect(makeTangentVec(vec1, vec2) == 0);

	cr_expect_float_eq(vec1[0], 3.0/5.0, tol);
	cr_expect_float_eq(vec1[1], 4.0/5.0, tol);

	cr_expect(makeTangentVec(vec1, vec3) == 0);

	cr_expect_float_eq(vec1[0], 1.0/sqrt(2.0), tol);
	cr_expect_float_eq(vec1[1], 1.0/sqrt(2.0), tol);
}


Test(testMakeTangentVec, testOutputValuesFor3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 	 = 1e-12;
	PetscScalar vec4[3];
	PetscScalar vec5[3]  = {1.0, 2.0, 3.0};
	PetscScalar vec6[3]  = {4.0, 5.0, 6.0};

	cr_expect(makeTangentVec(vec4, vec5) == 0);

	cr_expect_float_eq(vec4[0], 1.0/sqrt(14.0), tol);
	cr_expect_float_eq(vec4[1], 2.0/sqrt(14.0), tol);
	cr_expect_float_eq(vec4[2], 3.0/sqrt(14.0), tol);

	cr_expect(makeTangentVec(vec4, vec6) == 0);

	cr_expect_float_eq(vec4[0], 4.0/sqrt(77.0), tol);
	cr_expect_float_eq(vec4[1], 5.0/sqrt(77.0), tol);
	cr_expect_float_eq(vec4[2], 6.0/sqrt(77.0), tol);
}


Test(testMakePositionVec, testOutputValuesFor2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 	 		 =  1e-12;
	PetscScalar vec1[2];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzCoord[0] 		 =  4.9;
	node_ptr->xyzCoord[1] 		 =  3.8;

	cr_expect(makePositionVec(vec1, node_ptr) == 0);

	cr_expect_float_eq(vec1[0], 4.9, tol);
	cr_expect_float_eq(vec1[1], 3.8, tol);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


Test(testMakePositionVec, testOutputValuesFor3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 	 		 = 1e-12;
	PetscScalar vec2[3];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzCoord[0] 		 =  0.0;
	node_ptr->xyzCoord[1] 		 =  1.3;
	node_ptr->xyzCoord[2] 		 =  7.2;

	cr_expect(makePositionVec(vec2, node_ptr) == 0);

	cr_expect_float_eq(vec2[0], 0.0, tol);
	cr_expect_float_eq(vec2[1], 1.3, tol);
	cr_expect_float_eq(vec2[2], 7.2, tol);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


Test(testMakeDisplacementVec, testOutputValuesFor2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 	 		 =  1e-12;
	PetscScalar vec1[2];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzDisplacement[0] =  3.7;
	node_ptr->xyzDisplacement[1] = -2.9;

	cr_expect(makeDisplacementVec(vec1, node_ptr) == 0);

	cr_expect_float_eq(vec1[0],  3.7, tol);
	cr_expect_float_eq(vec1[1], -2.9, tol);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


Test(testMakeDisplacementVec, testOutputValuesFor3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 	 		 = 1e-12;
	PetscScalar vec2[3];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzDisplacement[0] =  4.0;
	node_ptr->xyzDisplacement[1] =  0.3;
	node_ptr->xyzDisplacement[2] = -7.2;

	cr_expect(makeDisplacementVec(vec2, node_ptr) == 0);

	cr_expect_float_eq(vec2[0], 4.0, tol);
	cr_expect_float_eq(vec2[1], 0.3, tol);
	cr_expect_float_eq(vec2[2],-7.2, tol);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


Test(testUpdatePositionVec, testOutputValuesFor2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 	 		 =  1e-12;
	PetscScalar vec1[2];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzCoord[0] 		 =  3.5;
	node_ptr->xyzCoord[1] 		 =  5.0;
	node_ptr->xyzDisplacement[0] =  1.3;
	node_ptr->xyzDisplacement[1] = -2.1;

	cr_expect(updatePositionVec(vec1, node_ptr) == 0);

	cr_expect_float_eq(vec1[0], 4.8, tol);
	cr_expect_float_eq(vec1[1], 2.9, tol);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


Test(testUpdatePositionVec, testOutputValuesFor3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 	 		 = 1e-12;
	PetscScalar vec2[3];
	Node *node_ptr 		 		 =  (Node *)malloc(sizeof(Node));
	node_ptr->xyzCoord[0] 		 =  0.0;
	node_ptr->xyzCoord[1] 		 =  4.3;
	node_ptr->xyzCoord[2] 		 =  9.2;
	node_ptr->xyzDisplacement[0] = -0.7;
	node_ptr->xyzDisplacement[1] =  0.0;
	node_ptr->xyzDisplacement[2] =  3.3;

	cr_expect(updatePositionVec(vec2, node_ptr) == 0);

	cr_expect_float_eq(vec2[0], -0.7, tol);
	cr_expect_float_eq(vec2[1],  4.3, tol);
	cr_expect_float_eq(vec2[2], 12.5, tol);

	/* clean up */
    free(node_ptr); node_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForPeriodicXIn2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 0) == 0);

	cr_expect_float_eq(vec1[0], 0.0, tol);
	cr_expect_float_eq(vec1[1], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForPeriodicXIn3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec2, box_ptr, 0) == 0);

	cr_expect_float_eq(vec2[0], 0.0, tol);
	cr_expect_float_eq(vec2[1], 2.0, tol);
	cr_expect_float_eq(vec2[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForAperiodicXIn2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 0) == 0);

	cr_expect_float_eq(vec1[0], 2.0, tol);
	cr_expect_float_eq(vec1[1], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForAperiodicXIn3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec2, box_ptr, 0) == 0);

	cr_expect_float_eq(vec2[0], 1.0, tol);
	cr_expect_float_eq(vec2[1], 2.0, tol);
	cr_expect_float_eq(vec2[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForPeriodicYIn2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 1;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 1) == 0);

	cr_expect_float_eq(vec1[0], 2.0, tol);
	cr_expect_float_eq(vec1[1], 0.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForPeriodicYIn3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 1;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec2, box_ptr, 1) == 0);

	cr_expect_float_eq(vec2[0], 1.0, tol);
	cr_expect_float_eq(vec2[1], 0.0, tol);
	cr_expect_float_eq(vec2[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForAperiodicYIn2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 1) == 0);

	cr_expect_float_eq(vec1[0], 2.0, tol);
	cr_expect_float_eq(vec1[1], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForAperiodicYIn3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec2, box_ptr, 1) == 0);

	cr_expect_float_eq(vec2[0], 1.0, tol);
	cr_expect_float_eq(vec2[1], 2.0, tol);
	cr_expect_float_eq(vec2[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForPeriodicZ)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 1;
	PetscScalar vec1[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 2) == 0);

	cr_expect_float_eq(vec1[0], 1.0, tol);
	cr_expect_float_eq(vec1[1], 2.0, tol);
	cr_expect_float_eq(vec1[2], 0.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForAperiodicZ)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec1[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 2) == 0);

	cr_expect_float_eq(vec1[0], 1.0, tol);
	cr_expect_float_eq(vec1[1], 2.0, tol);
	cr_expect_float_eq(vec1[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopy, testOutputValuesForPeriodicIn2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 1;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	cr_expect(nearestSegmentCopy(vec1, box_ptr) == 0);

	cr_expect_float_eq(vec1[0], 0.0, tol);
	cr_expect_float_eq(vec1[1], 0.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopy, testOutputValuesForPeriodicIn3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 1;
	box_ptr->xyzPeriodic[2] 	= 1;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopy(vec2, box_ptr) == 0);

	cr_expect_float_eq(vec2[0], 0.0, tol);
	cr_expect_float_eq(vec2[1], 0.0, tol);
	cr_expect_float_eq(vec2[2], 0.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopy, testOutputValuesForAPeriodicIn2D)
{
	if (DIMENSION != 2) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};

	cr_expect(nearestSegmentCopy(vec1, box_ptr) == 0);

	cr_expect_float_eq(vec1[0], 2.0, tol);
	cr_expect_float_eq(vec1[1], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopy, testOutputValuesForAPeriodicIn3D)
{
	if (DIMENSION != 3) cr_skip_test();

	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopy(vec2, box_ptr) == 0);

	cr_expect_float_eq(vec2[0], 1.0, tol);
	cr_expect_float_eq(vec2[1], 2.0, tol);
	cr_expect_float_eq(vec2[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}

