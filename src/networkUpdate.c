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
			ierr = updateInternalNodeDisp(node_ptr, box_ptr->nodeInternalCount, globalVec_U);CHKERRQ(ierr);
		}

		/* NOTE: don't need to update boundary nodes since they are updated
		 * before solving the system */
	}

	/* now update dangling nodes using updated internal nodes */
	PetscInt j;
	for (j = 0; j < box_ptr->fibreCount; j++)
	{
		Fibre *fibre_ptr = &(box_ptr->masterFibreList[j]);
		PetscInt nOnFibre = fibre_ptr->nodesOnFibre;

		ierr = checkForDanglingFibre(fibre_ptr, nOnFibre);CHKERRQ(ierr);
		
		if (nOnFibre > 2)
		{
			/* consider each end of the fibres node list */
			if (fibre_ptr->nodesOnFibreList[0]->nodeType == NODE_DANGLING)
			{
				ierr = updateDanglingNodeDisp(box_ptr, fibre_ptr->nodesOnFibreList[2], 
												fibre_ptr->nodesOnFibreList[1], 
												fibre_ptr->nodesOnFibreList[0]);
				CHKERRQ(ierr);
			}
			if (fibre_ptr->nodesOnFibreList[nOnFibre-1]->nodeType == NODE_DANGLING)
			{
				ierr = updateDanglingNodeDisp(box_ptr, fibre_ptr->nodesOnFibreList[nOnFibre-3], 
												fibre_ptr->nodesOnFibreList[nOnFibre-2], 
												fibre_ptr->nodesOnFibreList[nOnFibre-1]);
				CHKERRQ(ierr);
			}
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


/* Catches dangling fibres that should be avoided during network generation */
PetscErrorCode checkForDanglingFibre(Fibre *fibre_ptr, PetscInt nOnFibre)
{
	PetscErrorCode ierr = 0;

	if (nOnFibre == 3)
	{
		if (fibre_ptr->nodesOnFibreList[0]->nodeType == NODE_DANGLING &&
			fibre_ptr->nodesOnFibreList[nOnFibre-1]->nodeType == NODE_DANGLING)
		{
			ierr = PetscPrintf(PETSC_COMM_WORLD,"[WARNING] Found dangling fibre!\n");CHKERRQ(ierr);
		}
	}

	return ierr;
}


/* Updates a single dangling node using its 2 closest neighbouring nodes */
PetscErrorCode updateDanglingNodeDisp(Box *box_ptr, Node *alph_ptr, Node *beta_ptr, Node *delt_ptr)
{
	/* Vector operation summary:						*
	 * 		s_ndel = s_nbet + l_betaDelt * t_nalpBeta 	*
 	 *		u_delt = s_ndel - s_delt 					*/
	PetscErrorCode 	ierr = 0;
	PetscScalar 	l_betaDelt;

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

    ierr = makePositionVec(s_alph, alph_ptr);CHKERRQ(ierr);
    ierr = makePositionVec(s_beta, beta_ptr);CHKERRQ(ierr);
    ierr = makePositionVec(s_delt, delt_ptr);CHKERRQ(ierr);

    ierr = updatePositionVec(s_nalp, alph_ptr);CHKERRQ(ierr);
    ierr = updatePositionVec(s_nbet, beta_ptr);CHKERRQ(ierr);

    /* use distance vector to find previous segment length */
    ierr = makeDistanceVec(s_betaDelt, s_beta, s_delt, box_ptr);CHKERRQ(ierr);
    l_betaDelt = vecMagnitude(s_betaDelt);

    /* use new distance vector to find new orientation of dangling segment */
    ierr = makeDistanceVec(s_nalpBeta, s_nalp, s_nbet, box_ptr);CHKERRQ(ierr);
    ierr = makeTangentVec(t_nalpBeta, s_nalpBeta);CHKERRQ(ierr);

    /* combine new direction with previous magnitude to find new position */
    PetscInt i;
    for (i = 0; i < DIMENSION; i++)
    {
    	/* find new position */
    	s_ndel[i] = s_nbet[i] + (l_betaDelt * t_nalpBeta[i]);
    	/* convert it back to a displacement */
 	 	delt_ptr->xyzDisplacement[i] = s_ndel[i] - s_delt[i];
    }

	return ierr;
}

