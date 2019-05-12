#include "localAssemblyStretch.h"

/* Checks for legal fibre index and segment length */
void checkKArguments(Box *box_ptr, PetscInt fIndex, PetscScalar segLength)
{
    assert(fIndex >= 0);
    assert(segLength >= 0);
    assert(segLength < box_ptr->xyzDimension[0]*0.5 || box_ptr->xyzPeriodic[0] == 0);
    assert(segLength < box_ptr->xyzDimension[1]*0.5 || box_ptr->xyzPeriodic[1] == 0);
    assert(segLength < box_ptr->xyzDimension[2]*0.5 || box_ptr->xyzPeriodic[2] == 0);
}


/* Calculates the stretching constant k */
PetscScalar calculateK(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex, PetscScalar segLength)
{
    /* validate arguments */
    checkKArguments(box_ptr, fIndex, segLength);

    PetscScalar radius 	= box_ptr->masterFibreList[fIndex].radius;
    PetscScalar yMod 	= par_ptr->youngsModulus;
    PetscScalar area 	= M_PI * pow(radius, 2);
    PetscScalar mu 		= area * yMod;

    return mu / segLength;
}


PetscErrorCode calculateSegPairInfo( Box *box_ptr, Parameters *par_ptr, PetscScalar *s_alph, PetscScalar *s_beta,
        PetscScalar *k, PetscScalar *t_alphBeta, PetscInt fIndex )
{
    PetscErrorCode  ierr = 0;
    PetscScalar     l_alphBeta;
    PetscScalar     s_alphBeta[DIMENSION];

    /* make distance vector between position vectors */
    ierr = makeDistanceVec(s_alphBeta, s_alph, s_beta, box_ptr);CHKERRQ(ierr);

    /* make tangent vector of segment */
    ierr = makeTangentVec(t_alphBeta, s_alphBeta);CHKERRQ(ierr);

    /* calculate segment length */
    l_alphBeta = vecMagnitude(s_alphBeta);

    /* calculate stretching modulus */
    *k = calculateK(box_ptr, par_ptr, fIndex, l_alphBeta);

    return ierr;
}


/* Adds local stretch information for a single fibre to global system */
PetscErrorCode addFibreLocalStretch(Box *box_ptr, Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B, PetscInt fIndex)
{
    PetscErrorCode 	ierr = 0;
    PetscScalar		l_alphBeta, k;

    PetscBool       useLocalEM = PETSC_FALSE;
    PetscScalar     lambda = 1e-5;
    ierr = PetscOptionsGetBool(NULL,NULL,"-use_em",&useLocalEM,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsGetReal(NULL,NULL,"-k",&lambda,NULL);CHKERRQ(ierr);

    Fibre *fibre_ptr = &(box_ptr->masterFibreList[fIndex]);

    /* setup local matrix and rhs vector */
    PetscScalar localStretchMat_A[6][6];
    PetscScalar localStretchVec_b[6];        /* actual size will be 2*DIMENSION */

    /* setup static vectors */
    PetscScalar s_alph[DIMENSION];
    PetscScalar s_beta[DIMENSION];
    PetscScalar s_alphBeta[DIMENSION];
    PetscScalar t_alphBeta[DIMENSION];

    /* loop over every pair of nodes on the fibre */
    PetscInt i;
    for (i = 0; i < fibre_ptr->nodesOnFibre - 1; i++)
    {
        Node *alph_ptr = fibre_ptr->nodesOnFibreList[i];
        Node *beta_ptr = fibre_ptr->nodesOnFibreList[i+1];

        //ierr = calculateSegPairInfo(box_ptr, par_ptr, alph_ptr->xyzCoord, beta_ptr->xyzCoord, &k, t_alphBeta, fIndex);CHKERRQ(ierr);

        /* make position vectors for alpha and beta */
        ierr = makePositionVec(s_alph, alph_ptr);CHKERRQ(ierr);
        ierr = makePositionVec(s_beta, beta_ptr);CHKERRQ(ierr);

        /* make distance vector between position vectors */
        ierr = makeDistanceVec(s_alphBeta, s_alph, s_beta, box_ptr);CHKERRQ(ierr);

        //PetscScalar test[DIMENSION];
        //makeTangentVec(test, s_alphBeta);
        //assert(t_alphBeta[0] == test[0]);

        /* make tangent vector of segment */
        ierr = makeTangentVec(t_alphBeta, s_alphBeta);CHKERRQ(ierr);

        /* calculate segment length */
        l_alphBeta = vecMagnitude(s_alphBeta);

        //assert(k == calculateK(box_ptr, par_ptr, fIndex, l_alphBeta));

        /* calculate stretching modulus */
        k = calculateK(box_ptr, par_ptr, fIndex, l_alphBeta);

        //printf("k = %0.16g\n", k);
        //printf("t[0] = %0.16g\n", t_alphBeta[0]);
        //printf("t[1] = %0.16g\n", t_alphBeta[1]);
        //printf("t[2] = %0.16g\n", t_alphBeta[2]);

        PetscScalar *u_alph = alph_ptr->xyzDisplacement;    /* need to decide if it's safer to use makeDisplacementVec() here */
        PetscScalar *u_beta = beta_ptr->xyzDisplacement;

        if (DIMENSION == 2)
        {
            /* assemble the 2D local matrix and rhs vector */
            ierr = make2DStretchMat(k, t_alphBeta, localStretchMat_A);CHKERRQ(ierr);
            ierr = make2DStretchVec(u_alph, u_beta, k, t_alphBeta, localStretchVec_b);CHKERRQ(ierr);
        }
        else if (DIMENSION == 3)
        {
            /* assemble the 3D local matrix and rhs vector */
            ierr = make3DStretchMat(k, t_alphBeta, localStretchMat_A);CHKERRQ(ierr);
            ierr = make3DStretchVec(u_alph, u_beta, k, t_alphBeta, localStretchVec_b);CHKERRQ(ierr);
            if(useLocalEM)
            { 
                /*
                ierr = PetscPrintf(PETSC_COMM_WORLD,"Applying local stretch shift\n");CHKERRQ(ierr);
                ierr = applyMediumTo3DStretchMat(localStretchMat_A, lambda);CHKERRQ(ierr); 
                ierr = applyMediumTo3DStretchVec(localStretchVec_b, lambda, 
                alph_ptr->xyzAffDisplacement, 
                beta_ptr->xyzAffDisplacement);
                CHKERRQ(ierr); 
                */
            }
        }

        /* determine contributions and add to the global system */
        ierr = addStretchContToGlobal( alph_ptr, beta_ptr, globalMat_H, globalVec_B, 
                box_ptr->nodeInternalCount, localStretchMat_A, localStretchVec_b );
        CHKERRQ(ierr);
    }

    return ierr;
}


/* Assembles the 2D local stretch matrix of a given pair */
PetscErrorCode make2DStretchMat(PetscScalar k, PetscScalar *tangVec, PetscScalar localStretchMat_A[6][6])
{
    PetscErrorCode ierr = 0;

    PetscInt x=0;    /* set these values purely for readability */
    PetscInt y=1;    /* set *(1) also for readability */

    /* Row X (alpha) */
    localStretchMat_A[0][0] = ( 1) * k * pow( tangVec[x], 2 );
    localStretchMat_A[0][1] = (-1) * k * pow( tangVec[x], 2 );
    localStretchMat_A[0][2] = ( 1) * k * tangVec[x] * tangVec[y];
    localStretchMat_A[0][3] = (-1) * k * tangVec[x] * tangVec[y]; /* Additional values to be ignored
*   localStretchMat_A[0][4] = 0;  ******                           * in 2 dimensions
*   localStretchMat_A[0][5] = 0;  ******                           */

    /* Row X (beta) */
    localStretchMat_A[1][0] = localStretchMat_A[0][1];
    localStretchMat_A[1][1] = ( 1) * k * pow( tangVec[x], 2 );
    localStretchMat_A[1][2] = (-1) * k * tangVec[x] * tangVec[y];
    localStretchMat_A[1][3] = ( 1) * k * tangVec[x] * tangVec[y]; /* Additional values to be ignored
*   localStretchMat_A[1][4] = 0;  ******                           * in 2 dimensions
*   localStretchMat_A[1][5] = 0;  ******                           */

    /* Row Y (alpha) */
    localStretchMat_A[2][0] = localStretchMat_A[0][2];
    localStretchMat_A[2][1] = localStretchMat_A[1][2];
    localStretchMat_A[2][2] = ( 1) * k * pow( tangVec[y], 2 );
    localStretchMat_A[2][3] = (-1) * k * pow( tangVec[y], 2 );    /* Additional values to be ignored
*   localStretchMat_A[2][4] = 0;  ******                           * in 2 dimensions
*   localStretchMat_A[2][5] = 0;  ******                           */

    /* Row Y (beta) */
    localStretchMat_A[3][0] = localStretchMat_A[0][3];
    localStretchMat_A[3][1] = localStretchMat_A[1][3];
    localStretchMat_A[3][2] = localStretchMat_A[2][3];
    localStretchMat_A[3][3] = ( 1) * k * pow( tangVec[y], 2 );    /* Additional values to be ignored
*   localStretchMat_A[3][4] = 0;  ******                           * in 2 dimensions
*   localStretchMat_A[3][5] = 0;  ******                           */

    /* Row Z (alpha) */          /*
     *   localStretchMat_A[4][0] = 0;  ******
     *   localStretchMat_A[4][1] = 0;  ******
     *   localStretchMat_A[4][2] = 0;  ******
     *   localStretchMat_A[4][3] = 0;  ******
     *   localStretchMat_A[4][4] = 0;  ******
     *   localStretchMat_A[4][5] = 0;  */

    /* Row Z (beta) */           /*
     *   localStretchMat_A[5][0] = 0;  ******
     *   localStretchMat_A[5][1] = 0;  ******
     *   localStretchMat_A[5][2] = 0;  ******
     *   localStretchMat_A[5][3] = 0;  ******
     *   localStretchMat_A[5][4] = 0;  ******
     *   localStretchMat_A[5][5] = 0;  */

    return ierr;
}


/* Assembles the local 2D stretch RHS vector of a given pair */
PetscErrorCode make2DStretchVec( PetscScalar *u_alph, PetscScalar *u_beta, PetscScalar k, 
        PetscScalar *tangVec, PetscScalar *localStretchVec_b )
{
    PetscErrorCode ierr = 0;

    int x=0;    /* set these values purely for readability */
    int y=1;    /* set *(1) also for readability */

    PetscScalar extensionEqn = ((u_beta[x] - u_alph[x]) * tangVec[x]
            + (u_beta[y] - u_alph[y]) * tangVec[y]);

    /* we want the negation of the 1st partial derivatives */
    extensionEqn *= -1;

    localStretchVec_b[0] = (-1) * k * tangVec[x] * extensionEqn;
    localStretchVec_b[1] = ( 1) * k * tangVec[x] * extensionEqn;
    localStretchVec_b[2] = (-1) * k * tangVec[y] * extensionEqn;
    localStretchVec_b[3] = ( 1) * k * tangVec[y] * extensionEqn; /* Additional values to be ignored
*   localStretchVec_b[4] = 0;  ******                             * in 2 dimensions
*   localStretchVec_b[5] = 0;  ******                             */

    return ierr;
}


/* Assembles the 3D local stretch matrix of a given pair */
PetscErrorCode make3DStretchMat(PetscScalar k, PetscScalar *tangVec, PetscScalar localStretchMat_A[6][6])
{
    PetscErrorCode ierr = 0;

    PetscInt x=0;    /* set these values purely for readability */
    PetscInt y=1;    /* set *(1) also for readability */
    PetscInt z=2;

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

    return ierr;
}


/* Assembles the local 3D stretch RHS vector of a given pair */
PetscErrorCode make3DStretchVec( PetscScalar *u_alph, PetscScalar *u_beta, PetscScalar k, 
        PetscScalar *tangVec, PetscScalar *localStretchVec_b )
{
    PetscErrorCode ierr = 0;

    int x=0;    /* set these values purely for readability */
    int y=1;    /* set *(1) also for readability */
    int z=2;

    PetscScalar extensionEqn = ((u_beta[x] - u_alph[x]) * tangVec[x]
            + (u_beta[y] - u_alph[y]) * tangVec[y]
            + (u_beta[z] - u_alph[z]) * tangVec[z]);

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


PetscErrorCode applyMediumTo3DStretchMat(PetscScalar localStretchMat_A[6][6], const PetscScalar lambda)
{
    PetscErrorCode ierr = 0;
    assert(DIMENSION == 3);

    PetscInt i;
    for (i = 0; i < 2*DIMENSION; i++)
    {
        localStretchMat_A[i][i] += lambda;
    }
    return ierr;
}


PetscErrorCode applyMediumTo3DStretchVec(PetscScalar localStretchVec_b[6], const PetscScalar lambda, 
        const PetscScalar *u_aff_alph, const PetscScalar *u_aff_beta)
{
    PetscErrorCode ierr = 0;
    assert(DIMENSION == 3);

    PetscInt i;
    for (i = 0; i < DIMENSION; i++)
    {
        localStretchVec_b[2*i  ] += lambda * u_aff_alph[i];
        localStretchVec_b[2*i+1] += lambda * u_aff_beta[i];
    }
    return ierr;
}

