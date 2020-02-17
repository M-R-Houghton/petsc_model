#include "localAssemblyStretch.h"

/* Checks for legal fibre index and segment length */
void checkSegLength(const PetscScalar segLength, const PetscInt *xyzPer, const PetscScalar *xyzDim)
{
    assert(segLength >= 0);
    assert(segLength < xyzDim[0]*0.5 || xyzPer[0] == 0);
    assert(segLength < xyzDim[1]*0.5 || xyzPer[1] == 0);
    assert(segLength < xyzDim[2]*0.5 || xyzPer[2] == 0);
}


/* Calculates the stretching constant k */
PetscScalar calculateK(const PetscScalar radius, const PetscScalar youngsModulus, const PetscScalar segLength)
{
    PetscScalar area    = M_PI * pow(radius, 2);
    PetscScalar mu      = area * youngsModulus;
    return mu / segLength;
}


PetscErrorCode calculateSegPairInfo( PetscScalar *k, PetscScalar *t_alphBeta, const Fibre *fibre_ptr, 
                                        const PetscScalar *s_alph, const PetscScalar *s_beta,
                                        const PetscInt *xyzPer, const PetscScalar *xyzDim, const PetscScalar youngsModulus)
{
    PetscErrorCode  ierr = 0;
    PetscScalar     l_alphBeta;
    PetscScalar     s_alphBeta[DIMENSION];

    /* make distance vector between position vectors */
    ierr = posVecDifference(s_alphBeta, s_alph, s_beta, xyzPer, xyzDim);CHKERRQ(ierr);

    /* make tangent vector of segment */
    ierr = makeTangentVec(t_alphBeta, s_alphBeta);CHKERRQ(ierr);

    /* calculate segment length */
    l_alphBeta = vecMagnitude(s_alphBeta);
    checkSegLength(l_alphBeta, xyzPer, xyzDim);

    /* calculate stretching modulus */
    *k = calculateK(fibre_ptr->radius, youngsModulus, l_alphBeta);

    return ierr;
}


/* Adds local stretch information for a single fibre to global system */
PetscErrorCode addFibreLocalStretch(Mat globalMat_H, Vec globalVec_B, const PetscInt N, const Fibre *fibre_ptr, 
                                    const PetscInt *xyzPer, const PetscScalar *xyzDim, const PetscScalar youngsModulus)
{
    PetscErrorCode  ierr = 0;
    PetscScalar     l_alphBeta, k;

    /* setup local matrix and rhs vector */
    PetscScalar localStretchMat_A[6][6];
    PetscScalar localStretchVec_b[6];        /* actual size will be 2*DIMENSION */

    /* setup static vectors */
    PetscScalar s_alphBeta[DIMENSION];
    PetscScalar t_alphBeta[DIMENSION];
    PetscScalar u_alphBeta[DIMENSION];

    /* loop over every pair of nodes on the fibre */
    PetscInt i;
    for (i = 0; i < fibre_ptr->nodesOnFibre - 1; i++)
    {
        const Node *n_alph = fibre_ptr->nodesOnFibreList[i];
        const Node *n_beta = fibre_ptr->nodesOnFibreList[i+1];

        // TODO: Decide whether this function is worth using
        //ierr = calculateSegPairInfo( &k, t_alphBeta, fibre_ptr, 
        //                              n_alph->xyzCoord, n_beta->xyzCoord, 
        //                              xyzPer, xyzDim, youngsModulus );
        //CHKERRQ(ierr);

        /* make distance vector between position vectors */
        ierr = posVecDifference(s_alphBeta, n_alph->xyzCoord, n_beta->xyzCoord, xyzPer, xyzDim);CHKERRQ(ierr);

        /* make tangent vector of segment */
        ierr = makeTangentVec(t_alphBeta, s_alphBeta);CHKERRQ(ierr);

        /* calculate segment length */
        l_alphBeta = vecMagnitude(s_alphBeta);
        checkSegLength(l_alphBeta, xyzPer, xyzDim);

        /* calculate stretching modulus */
        k = calculateK(fibre_ptr->radius, youngsModulus, l_alphBeta);

        ierr = stdVecDifference(u_alphBeta, n_alph->xyzDisplacement, n_beta->xyzDisplacement);CHKERRQ(ierr);

        if (DIMENSION == 2)
        {
            /* assemble the 2D local matrix and rhs vector */
            ierr = make2DStretchMat(k, t_alphBeta, localStretchMat_A);CHKERRQ(ierr);
            ierr = make2DStretchVec(u_alphBeta, k, t_alphBeta, localStretchVec_b);CHKERRQ(ierr);
        }
        else if (DIMENSION == 3)
        {
            /* assemble the 3D local matrix and rhs vector */
            ierr = make3DStretchMat(k, t_alphBeta, localStretchMat_A);CHKERRQ(ierr);
            ierr = make3DStretchVec(u_alphBeta, k, t_alphBeta, localStretchVec_b);CHKERRQ(ierr);
        }

        /* determine contributions and add to the global system */
        ierr = addStretchContToGlobal( globalMat_H, globalVec_B, N, 
                                        n_alph->globalID, n_beta->globalID, 
                                        n_alph->nodeType, n_beta->nodeType, 
                                        localStretchMat_A, localStretchVec_b );
        CHKERRQ(ierr);
    }

    return ierr;
}


/* Assembles the 2D local stretch matrix of a given pair */
PetscErrorCode make2DStretchMat(const PetscScalar k, const PetscScalar *tangVec, PetscScalar localStretchMat_A[6][6])
{
    PetscErrorCode ierr = 0;

    const PetscInt x=0;    /* set these values purely for readability */
    const PetscInt y=1;    /* set *(1) also for readability */

    /* Row X (alpha) */
    localStretchMat_A[0][0] = ( 1) * k * pow( tangVec[x], 2 );
    localStretchMat_A[0][1] = (-1) * k * pow( tangVec[x], 2 );
    localStretchMat_A[0][2] = ( 1) * k * tangVec[x] * tangVec[y];
    localStretchMat_A[0][3] = (-1) * k * tangVec[x] * tangVec[y]; 

    /* Row X (beta) */
    localStretchMat_A[1][0] = localStretchMat_A[0][1];
    localStretchMat_A[1][1] = ( 1) * k * pow( tangVec[x], 2 );
    localStretchMat_A[1][2] = (-1) * k * tangVec[x] * tangVec[y];
    localStretchMat_A[1][3] = ( 1) * k * tangVec[x] * tangVec[y]; 

    /* Row Y (alpha) */
    localStretchMat_A[2][0] = localStretchMat_A[0][2];
    localStretchMat_A[2][1] = localStretchMat_A[1][2];
    localStretchMat_A[2][2] = ( 1) * k * pow( tangVec[y], 2 );
    localStretchMat_A[2][3] = (-1) * k * pow( tangVec[y], 2 );    

    /* Row Y (beta) */
    localStretchMat_A[3][0] = localStretchMat_A[0][3];
    localStretchMat_A[3][1] = localStretchMat_A[1][3];
    localStretchMat_A[3][2] = localStretchMat_A[2][3];
    localStretchMat_A[3][3] = ( 1) * k * pow( tangVec[y], 2 );    

    /* 
     * NOTE: The remaining values are not set and should be ignored
     *  localStretchMat_A[0:3][4:5] = N/A
     *  localStretchMat_A[4:5][0:5] = N/A
     */

    return ierr;
}


/* Assembles the local 2D stretch RHS vector of a given pair */
PetscErrorCode make2DStretchVec( const PetscScalar *u_alphBeta, const PetscScalar k, 
                                    const PetscScalar *tangVec, PetscScalar *localStretchVec_b )
{
    PetscErrorCode ierr = 0;

    const PetscInt x=0;    /* set these values purely for readability */
    const PetscInt y=1;    /* set *(1) also for readability */

    PetscScalar extensionEqn = (u_alphBeta[x] * tangVec[x] + u_alphBeta[y] * tangVec[y]);

    /* we want the negation of the 1st partial derivatives */
    extensionEqn *= -1;

    localStretchVec_b[0] = (-1) * k * tangVec[x] * extensionEqn;
    localStretchVec_b[1] = ( 1) * k * tangVec[x] * extensionEqn;
    localStretchVec_b[2] = (-1) * k * tangVec[y] * extensionEqn;
    localStretchVec_b[3] = ( 1) * k * tangVec[y] * extensionEqn; 

    /* 
     * NOTE: The remaining values are not set and should be ignored
     *  localStretchVec_b[4:5] = N/A
     */

    return ierr;
}


/* Assembles the 3D local stretch matrix of a given pair */
PetscErrorCode make3DStretchMat(const PetscScalar k, const PetscScalar *tangVec, PetscScalar localStretchMat_A[6][6])
{
    PetscErrorCode ierr = 0;

    const PetscInt x=0;    /* set these values purely for readability */
    const PetscInt y=1;    /* set *(1) also for readability */
    const PetscInt z=2;

    /* Row X (alpha) */
    localStretchMat_A[0][0] = ( 1) * k * pow( tangVec[x], 2 );
    localStretchMat_A[0][1] = (-1) * k * pow( tangVec[x], 2 );
    localStretchMat_A[0][2] = ( 1) * k * tangVec[x] * tangVec[y];
    localStretchMat_A[0][3] = (-1) * k * tangVec[x] * tangVec[y];
    localStretchMat_A[0][4] = ( 1) * k * tangVec[x] * tangVec[z];
    localStretchMat_A[0][5] = (-1) * k * tangVec[x] * tangVec[z];

    /* Row X (beta) */
    localStretchMat_A[1][0] = localStretchMat_A[0][1];
    localStretchMat_A[1][1] = ( 1) * k * pow( tangVec[x], 2 );
    localStretchMat_A[1][2] = (-1) * k * tangVec[x] * tangVec[y];
    localStretchMat_A[1][3] = ( 1) * k * tangVec[x] * tangVec[y];
    localStretchMat_A[1][4] = (-1) * k * tangVec[x] * tangVec[z];
    localStretchMat_A[1][5] = ( 1) * k * tangVec[x] * tangVec[z];

    /* Row Y (alpha) */
    localStretchMat_A[2][0] = localStretchMat_A[0][2];
    localStretchMat_A[2][1] = localStretchMat_A[1][2];
    localStretchMat_A[2][2] = ( 1) * k * pow( tangVec[y], 2 );
    localStretchMat_A[2][3] = (-1) * k * pow( tangVec[y], 2 );
    localStretchMat_A[2][4] = ( 1) * k * tangVec[y] * tangVec[z];
    localStretchMat_A[2][5] = (-1) * k * tangVec[y] * tangVec[z];

    /* Row Y (beta) */
    localStretchMat_A[3][0] = localStretchMat_A[0][3];
    localStretchMat_A[3][1] = localStretchMat_A[1][3];
    localStretchMat_A[3][2] = localStretchMat_A[2][3];
    localStretchMat_A[3][3] = ( 1) * k * pow( tangVec[y], 2 );
    localStretchMat_A[3][4] = (-1) * k * tangVec[y] * tangVec[z];
    localStretchMat_A[3][5] = ( 1) * k * tangVec[y] * tangVec[z];

    /* Row Z (alpha) */
    localStretchMat_A[4][0] = localStretchMat_A[0][4];
    localStretchMat_A[4][1] = localStretchMat_A[1][4];
    localStretchMat_A[4][2] = localStretchMat_A[2][4];
    localStretchMat_A[4][3] = localStretchMat_A[3][4];
    localStretchMat_A[4][4] = ( 1) * k * pow( tangVec[z], 2 );
    localStretchMat_A[4][5] = (-1) * k * pow( tangVec[z], 2 );

    /* Row Z (beta) */
    localStretchMat_A[5][0] = localStretchMat_A[0][5];
    localStretchMat_A[5][1] = localStretchMat_A[1][5];
    localStretchMat_A[5][2] = localStretchMat_A[2][5];
    localStretchMat_A[5][3] = localStretchMat_A[3][5];
    localStretchMat_A[5][4] = localStretchMat_A[4][5];
    localStretchMat_A[5][5] = ( 1) * k * pow( tangVec[z], 2 );

    int i,j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"[SMAT] %0.16g\t", localStretchMat_A[i][j]);CHKERRQ(ierr);
        }
        ierr = PetscPrintf(PETSC_COMM_WORLD, "\n");CHKERRQ(ierr);
    }
    ierr = PetscPrintf(PETSC_COMM_WORLD, "\n");CHKERRQ(ierr);

    return ierr;
}


/* Assembles the local 3D stretch RHS vector of a given pair */
PetscErrorCode make3DStretchVec( const PetscScalar *u_alphBeta, const PetscScalar k, 
                                    const PetscScalar *tangVec, PetscScalar *localStretchVec_b )
{
    PetscErrorCode ierr = 0;

    const PetscInt x=0;    /* set these values purely for readability */
    const PetscInt y=1;    /* set *(1) also for readability */
    const PetscInt z=2;

    PetscScalar extensionEqn = u_alphBeta[x]*tangVec[x] + u_alphBeta[y]*tangVec[y] + u_alphBeta[z]*tangVec[z];

    /* we want the negation of the 1st partial derivatives */
    extensionEqn *= -1;

    localStretchVec_b[0] = (-1) * k * tangVec[x] * extensionEqn;
    localStretchVec_b[1] = ( 1) * k * tangVec[x] * extensionEqn;
    localStretchVec_b[2] = (-1) * k * tangVec[y] * extensionEqn;
    localStretchVec_b[3] = ( 1) * k * tangVec[y] * extensionEqn;
    localStretchVec_b[4] = (-1) * k * tangVec[z] * extensionEqn;
    localStretchVec_b[5] = ( 1) * k * tangVec[z] * extensionEqn;

    return ierr;
}


