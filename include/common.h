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
	double gamma;
	double youngsModulus;
};

/* structure for the domain */
struct box 
{
	int nodeCount;
	int nodeInternalCount;
	int fibreCount;
	double xyzDimension[DIMENSION];
	int xyzPeriodic[DIMENSION];
	Node *masterNodeList;			/* declare lists for storing nodes and fibres */
	Fibre *masterFibreList;
};

/* structure for nodes */
struct node 
{
	int nodeID;
	int nodeType;					/* might want to link back later */
	int globalID;
	double xyzCoord[DIMENSION];
	double xyzDisplacement[DIMENSION];
};

/* structure for fibres */
struct fibre 
{
	int fibreID;
	int nodesOnFibre;
	double radius;
	Node **nodesOnFibreList;
};

/* structure for sparse matrices */
struct sparseMat 
{
	int n;
	int nz;
	int *counter; 					/* (0<n)   counters     */
	int *rowp;    					/* (0<n+1) row pointers */
	int *col;     					/* (0<nz)  column index */
	double *mat;  					/* (0<nz)  matrix entry */
};

#endif