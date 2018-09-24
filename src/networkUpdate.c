#include "networkUpdate.h"

/* Updates the network with displacement found from solve */
PetscErrorCode networkUpdate(Box *box_ptr, Vec globalVec_U)
{
	PetscErrorCode ierr = 0;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Updating network...\n");CHKERRQ(ierr);

	ierr = VecView(globalVec_U,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/* update network by looping node by node */
	PetscInt i;
	for (i = 0; i < box_ptr->nodeCount; i++)
	{
		Node *node_ptr = &(box_ptr->masterNodeList[i]);

		/* update all internal nodes before updating dangling nodes */
		if (node_ptr->nodeType == NODE_INTERNAL)
		{
			updateInternalNodeDisp(node_ptr, box_ptr->nodeInternalCount, globalVec_U);
		}

		/* NOTE: don't need to update boundary nodes since they are updated
		 * before solving the system */
	}

	/* now update dangling nodes using updated internal nodes */
	PetscInt j;
	for (j = 0; j < box_ptr->fibreCount; j++)
	{
		Fibre *fibre_ptr = &(box_ptr->masterFibreList[j]);

		if (fibre_ptr->nodesOnFibre > 2)
		{
			/* consider each end of the fibres node list */
		}
	}

	return ierr;
}


/* Updates a single internal node with its relevant solved displacement */
PetscErrorCode updateInternalNodeDisp(Node *node_ptr, PetscInt N, Vec globalVec_U)
{
	PetscErrorCode 	ierr = 0;
	PetscScalar 	*solArray;

	/* NOTE: I need to investigate the performance here.
	 * 	It may be more sensible to call VecGetArray() once before looping over
	 *	all the nodes and call VecRestoreArray() once after leaving the loop. */

	/* Check out array */
    ierr = VecGetArray(globalVec_U, &solArray);CHKERRQ(ierr);
    
    PetscInt i;
    for (i = 0; i < DIMENSION; i++)
    {
    	node_ptr->xyzDisplacement[i] += solArray[node_ptr->globalID + i*N];
    }
    
    /* Check array back in */
    ierr = VecRestoreArray(globalVec_U, &solArray);CHKERRQ(ierr);

	return ierr;
}


/* Updates a single dangling node using its 2 closest neighbouring nodes */
PetscErrorCode updateDanglingNodeDisp(Box *box_ptr, Node *alph_ptr, Node *beta_ptr, Node *delt_ptr)
{
	/* Vector operation summary:						*
	 * 		s_ndel = s_nbet + l_betaDelt * t_nalpBeta 	*
 	 *		u_delt = s_ndel - s_delt 					*/
	PetscErrorCode ierr = 0;

	/* initialise initial position vectors */
    PetscScalar s_alph[DIMENSION];
    PetscScalar s_beta[DIMENSION];
    PetscScalar s_delt[DIMENSION];

    /* initialise new position vectors */
    PetscScalar s_nalp[DIMENSION];
    PetscScalar s_nbet[DIMENSION];
    PetscScalar s_ndel[DIMENSION];

    /* initialise other vectors */
    PetscScalar s_betaDelt[DIMENSION];
    PetscScalar s_nalpBeta[DIMENSION];
    PetscScalar t_nalpBeta[DIMENSION];

    /* make position vectors */
    ierr = makePositionVec(s_alph, alph_ptr);CHKERRQ(ierr);
    ierr = makePositionVec(s_beta, beta_ptr);CHKERRQ(ierr);
    ierr = makePositionVec(s_delt, delt_ptr);CHKERRQ(ierr);

    /* make updated position vectors */
    ierr = updatePositionVec(s_nalp, alph_ptr);CHKERRQ(ierr);
    ierr = updatePositionVec(s_nbet, beta_ptr);CHKERRQ(ierr);

    /* make distance vector */
    ierr = makeDistanceVec(s_betaDelt, s_beta, s_delt, box_ptr);CHKERRQ(ierr);

	return ierr;
}

