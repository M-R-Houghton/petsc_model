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
PetscErrorCode vec2DCrossProduct(PetscScalar *crossVec_ptr, PetscScalar *vec1_ptr, PetscScalar *vec2_ptr)
{
	assert(DIMENSION == 2);		/* should not be calling this function in 3d case */

	PetscErrorCode ierr = 0;
	crossVec_ptr[0] 	= 0;
	crossVec_ptr[1] 	= 0;
	crossVec_ptr[2] 	= vec1_ptr[0]*vec2_ptr[1] - vec1_ptr[1]*vec2_ptr[0];

	return ierr;
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
PetscErrorCode vecAddition(PetscScalar *addVec_ptr, const PetscScalar *posVec1_ptr, const PetscScalar *posVec2_ptr, 
                            const PetscInt *xyzPeriodic, const PetscScalar *xyzDimension)
{
    //TODO: Add safety check for accidental periodicity/dimension mix up
	PetscErrorCode ierr = 0;

	int i;
	for (i = 0; i < DIMENSION; i++)
	{
		addVec_ptr[i] = posVec1_ptr[i] + posVec2_ptr[i];
	}

	/* update if crossing boundary */
	ierr = nearestSegmentCopy(addVec_ptr, xyzPeriodic, xyzDimension);CHKERRQ(ierr);

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
	ierr = nearestSegmentCopy(distVec_ptr, box_ptr->xyzPeriodic, box_ptr->xyzDimension);CHKERRQ(ierr);

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
PetscErrorCode nearestSegmentCopyDirN(PetscScalar *distVec_ptr, const PetscInt N, const PetscInt perN, const PetscScalar dimN)
{
	PetscErrorCode ierr = 0;

	if (perN == 1)
	{
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
PetscErrorCode nearestSegmentCopy(PetscScalar *distVec_ptr, const PetscInt *xyzPer, const PetscScalar *xyzDim)
{
	PetscErrorCode  ierr = 0;
    PetscInt        i;

    // TODO: Check whether it is sufficient to loop over DIMENSION instead of up to i
    for (i = 0; i < 3; i++)
    {
        ierr = nearestSegmentCopyDirN(distVec_ptr, i, xyzPer[i], xyzDim[i]);CHKERRQ(ierr);
    }

	return ierr;
}


