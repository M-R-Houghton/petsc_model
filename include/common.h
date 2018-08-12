#ifndef COMMON_H
#define COMMON_H

#include <petscksp.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "minUnitMacros.h"

#define DIMENSION 3
#define MAX_LENGTH 10000
#define PARAMETERS 8
#define NODE_INTERNAL 0
#define NODE_DANGLING 1
#define NODE_BOUNDARY 2

/* 
 * set the span of association between nodes
 * stretch only     = 1
 * stretch and bend = 2
 */
#define SPAN 2

typedef struct parameters Parameters;
typedef struct box Box;
typedef struct node Node;
typedef struct fibre Fibre;
typedef struct sparseMat Sparse;

/* structure for storing variables */
struct parameters 
{
	PetscScalar gamma;
	PetscScalar youngsModulus;
};

/* structure for the domain */
struct box 
{
	PetscInt nodeCount;
	PetscInt nodeInternalCount;
	PetscInt fibreCount;
	PetscScalar xyzDimension[3];
	PetscInt xyzPeriodic[3];
	Node *masterNodeList;			/* declare lists for storing nodes and fibres */
	Fibre *masterFibreList;
};

/* structure for nodes */
struct node 
{
	PetscInt nodeID;
	PetscInt nodeType;					/* might want to link back later */
	PetscInt globalID;
	PetscScalar xyzCoord[3];
	PetscScalar xyzDisplacement[3];
};

/* structure for fibres */
struct fibre 
{
	PetscInt fibreID;
	PetscInt nodesOnFibre;
	PetscScalar radius;
	Node **nodesOnFibreList;
};

/* structure for sparse matrices */
struct sparseMat 
{
	PetscInt n;
	PetscInt nz;
	PetscInt *counter; 					/* (0<n)   counters     */
	PetscInt *rowp;    					/* (0<n+1) row pointers */
	PetscInt *col;     					/* (0<nz)  column index */
	PetscScalar *mat;  					/* (0<nz)  matrix entry */
};

#endif