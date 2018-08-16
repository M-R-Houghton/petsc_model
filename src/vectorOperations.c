#include "vectorOperations.h"

/* Calculates the minimum of two integers */
PetscInt minInt(PetscInt a, PetscInt b)
{
	if (a < b) 
	{
		return a;
	}
	return b;
}


/* Calculates the maximum of two integers */
PetscInt maxInt(PetscInt a, PetscInt b)
{
	if (a > b) 
	{
		return a;
	}
	return b;
}


/* Calculates the minimum of two scalars */
PetscScalar minScalar(PetscScalar a, PetscScalar b)
{
	if (a < b) 
	{
		return a;
	}
	return b;
}


/* Calculates the maximum of two scalars */
PetscScalar maxScalar(PetscScalar a, PetscScalar b)
{
	if (a > b) 
	{
		return a;
	}
	return b;
}


/* Calculates the dot product of two given vectors */
PetscScalar vecDotProduct(PetscScalar *vec1_ptr, PetscScalar *vec2_ptr)
{
	PetscScalar total = 0;
	int i;
	for (i = 0; i < DIMENSION; i++)
	{
		total += vec1_ptr[i] * vec2_ptr[i];
	}
	return total;
}


/* Calculates the cross product of two given 2D vectors */
PetscScalar vec2DCrossProduct(PetscScalar *vec1_ptr, PetscScalar *vec2_ptr)
{
	assert(DIMENSION == 2);		/* should not be calling this function in 3d case */

	PetscScalar z = vec1_ptr[0] * vec2_ptr[1] - vec2_ptr[0] * vec1_ptr[1];
	return z;
}


/* Calculates the cross product of two given 3D vectors */
PetscErrorCode vec3DCrossProduct(PetscScalar *crossVec_ptr, PetscScalar *vec1_ptr, PetscScalar *vec2_ptr)
{
	assert(DIMENSION == 3);		/* should not be calling this function in 2d case */

	PetscErrorCode ierr = 0;
	crossVec_ptr[0] 	= vec1_ptr[1]*vec2_ptr[2] - vec1_ptr[2]*vec2_ptr[1];
	crossVec_ptr[1] 	= vec1_ptr[2]*vec2_ptr[0] - vec1_ptr[0]*vec2_ptr[2];
	crossVec_ptr[2] 	= vec1_ptr[0]*vec2_ptr[1] - vec1_ptr[1]*vec2_ptr[0];

	return ierr;
}


/* Calculates the magnitude of a given vector */
PetscScalar vecMagnitude(PetscScalar *vec_ptr)
{
	return sqrt(vecDotProduct(vec_ptr, vec_ptr));
}


/* Calculates the addition of two position vectors */
PetscErrorCode vecAddition(PetscScalar *addVec_ptr, PetscScalar *posVec1_ptr, PetscScalar *posVec2_ptr, Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	int i;
	for (i = 0; i < DIMENSION; i++)
	{
		addVec_ptr[i] = posVec1_ptr[i] + posVec2_ptr[i];
	}

	/* update if crossing boundary */
	ierr = nearestSegmentCopy(addVec_ptr, box_ptr);CHKERRQ(ierr);

	return ierr;
}


/* Creates the distance vector between two position vectors */
PetscErrorCode makeDistanceVec(PetscScalar *distVec_ptr, PetscScalar *posVec1_ptr, PetscScalar *posVec2_ptr, Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	int i;
	for (i = 0; i < DIMENSION; i++)
	{
		distVec_ptr[i] = posVec2_ptr[i] - posVec1_ptr[i];
	}

	/* update if crossing boundary */
	ierr = nearestSegmentCopy(distVec_ptr, box_ptr);CHKERRQ(ierr);

	return ierr;
}


/* Creates the unit tangent vector of a given vector */
PetscErrorCode makeTangentVec(PetscScalar *tangVec_ptr, PetscScalar *vec_ptr)
{
	PetscErrorCode ierr = 0;

	int i;
	for (i = 0; i < DIMENSION; i++)
	{
		tangVec_ptr[i] = vec_ptr[i] / (vecMagnitude(vec_ptr));
	}

	return ierr;
}


/* Creates the position vector of a given node */
PetscErrorCode makePositionVec(PetscScalar *posVec_ptr, Node *node_ptr)
{
	PetscErrorCode ierr = 0;

	int i;
	for (i = 0; i < DIMENSION; i++)
	{
		posVec_ptr[i] = node_ptr->xyzCoord[i];
	}

	return ierr;
}


/* Creates the displacement vector of a given node */
PetscErrorCode makeDisplacementVec(PetscScalar *dispVec_ptr, Node *node_ptr)
{
	PetscErrorCode ierr = 0;

	int i;
	for (i = 0; i < DIMENSION; i++)
	{
		dispVec_ptr[i] = node_ptr->xyzDisplacement[i];
	}

	return ierr;
}


/* Updates a position vector with the displacement of the corresponding node */
PetscErrorCode updatePositionVec(PetscScalar *posVec_ptr, Node *node_ptr)
{
	PetscErrorCode ierr = 0;

	int i;
	for (i = 0; i < DIMENSION; i++)
	{
		posVec_ptr[i] = node_ptr->xyzCoord[i] + node_ptr->xyzDisplacement[i];
	}

	return ierr;
}


/* Checks whether a segment crosses the N boundary and updates it to the nearest copy inside the domain */
PetscErrorCode nearestSegmentCopyDirN(PetscScalar *distVec_ptr, Box *box_ptr, PetscInt N)
{
	PetscErrorCode ierr = 0;

	if (box_ptr->xyzPeriodic[N] == 1)
	{
		PetscScalar dimN = box_ptr->xyzDimension[N]; 	/* i.e. width/height/depth */

		while (distVec_ptr[N] >  0.5 * dimN) 
		{
			distVec_ptr[N] -= dimN;
		} 
		while (distVec_ptr[N] < -0.5 * dimN) 
		{
			distVec_ptr[N] += dimN;
		}
	}
	return ierr;
}


/* Checks whether a segment crosses any boundary and updates it to the nearest copy inside the domain */
PetscErrorCode nearestSegmentCopy(PetscScalar *distVec_ptr, Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	/* check if x-periodic */
	ierr = nearestSegmentCopyDirN(distVec_ptr, box_ptr, 0);CHKERRQ(ierr);

	/* check if y-periodic */
	ierr = nearestSegmentCopyDirN(distVec_ptr, box_ptr, 1);CHKERRQ(ierr);

	/* check if z-periodic */
	ierr = nearestSegmentCopyDirN(distVec_ptr, box_ptr, 2);CHKERRQ(ierr);

	return ierr;
}


