#ifndef COMMON_H
#define COMMON_H

#include <petscksp.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "minUnitMacros.h"

#define DIMENSION 3
#define MAX_LENGTH 10000
#define MAX_NAME 100
#define MAX_NODES_ON_COUPLE 200
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

/* may remove and add back into .par files */
/* WARNING: These are now overwritten in main.c */
#define GAMMA 0.02
#define YOUNGS_MOD 1.0
/* CHECK FOR DEPENDENCIES AND REMOVE */

typedef struct parameters Parameters;
typedef struct box Box;
typedef struct node Node;
typedef struct fibre Fibre;
typedef struct couple Couple;
typedef struct sparseMat Sparse;

/* structure for storing variables */
struct parameters 
{
    char inputNetwork[MAX_NAME];
    char outputNetwork[MAX_NAME];
    char outadvNetwork[MAX_NAME];
    char postSolveResults[MAX_NAME];
    PetscScalar gamma;
    PetscScalar youngsModulus;
    PetscScalar energyStre;
    PetscScalar energyBend;
    PetscScalar energyTotl;
    PetscScalar energyPsAf;
    PetscScalar energyAffn;
    PetscScalar shearModulus;
    PetscScalar shearModAffn;
    /* sheet based network parameters */
    PetscScalar inPlnEnergyTotl;
    PetscScalar inPlnEnergyAffn;
    PetscScalar outPlnEnergyTotl;
    PetscScalar outPlnEnergyAffn;
};

/* structure for the domain */
struct box 
{
    PetscInt nodeCount;
    PetscInt nodeInternalCount;
    PetscInt fibreCount;
    PetscInt coupleCount;           /* in many cases this may be equal to internal */
    PetscScalar xyzDimension[3];
    PetscInt xyzPeriodic[3];
    Node *masterNodeList;           /* declare lists for storing nodes and fibres */
    Fibre *masterFibreList;
    Couple *masterCoupleList;

    /* probably want to move this into its own struct eventually */
    PetscInt sheetCount;
    PetscInt fibreCountPerSheet;
    PetscInt conFibCountPerSheetPair;
};

/* structure for coupled nodes */
struct couple
{
    PetscInt coupleID;
    PetscInt nodesInCouple; 
    PetscInt nodeID[MAX_NODES_ON_COUPLE];

    /* TODO: change to nodesOnCouple for consistency */
    /* TODO: change to dynamically allocated nodeIDList */
    //PetscInt *nodeIDList;
};

/* structure for nodes */
struct node 
{
    PetscInt nodeID;
    PetscInt nodeType;                  /* might want to link back later */
    PetscInt globalID;
    PetscScalar xyzCoord[3];
    PetscScalar xyzDisplacement[3];
    PetscScalar xyzAffDisplacement[3];
};

/* structure for fibres */
struct fibre 
{
    PetscInt fibreID;
    PetscInt nodesOnFibre;
    PetscScalar radius;
    Node **nodesOnFibreList;

    PetscScalar fibreStreEnergy;
    PetscScalar fibreBendEnergy;
    PetscScalar fibrePsAfEnergy;
    PetscScalar fibreAffnEnergy;
};

/* structure for sparse matrices */
struct sparseMat 
{
    PetscInt n;
    PetscInt nz;
    PetscInt *counter;                  /* (0<n)   counters     */
    PetscInt *rowp;                     /* (0<n+1) row pointers */
    PetscInt *col;                      /* (0<nz)  column index */
    PetscScalar *mat;                   /* (0<nz)  matrix entry */
};

/* set macros for compatibility between 3.4 and 3.10 */
#if (PETSC_VERSION_MAJOR==3 && PETSC_VERSION_MINOR<5)
#define MATSTRUCT , SAME_NONZERO_PATTERN
#define GETOPTS 
#else
#define MATSTRUCT
#define GETOPTS NULL, 
#endif

#endif
