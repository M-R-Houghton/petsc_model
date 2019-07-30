#include "networkWrite.h"

/* Initiates network write out routine */
PetscErrorCode networkWrite(const char *fileName, Box *box_ptr)
{
	PetscErrorCode 	ierr;
	PetscInt fIndex, nIndex;
	FILE *file_ptr;

	file_ptr = fopen(fileName, "w+");

	/* first written line should be box line */
	ierr = writeBoxLine(file_ptr, box_ptr);CHKERRQ(ierr);

	/* followed a line for every fibre */
	for (fIndex = 0; fIndex < box_ptr->fibreCount; fIndex++)
	{
		ierr = writeFibreLine(file_ptr, &(box_ptr->masterFibreList[fIndex]));CHKERRQ(ierr);
	}

	/* ending with a line for every node */
	for (nIndex = 0; nIndex < box_ptr->nodeCount; nIndex++)
	{
		ierr = writeNodeLine(file_ptr, &(box_ptr->masterNodeList[nIndex]));CHKERRQ(ierr);
	}

	ierr = fclose(file_ptr);

	return ierr;
}


PetscErrorCode printFibreInfo(Fibre *fibre_ptr)
{
    PetscErrorCode ierr;
    
    ierr = PetscPrintf(PETSC_COMM_WORLD,"FIBRE INFO\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"----------\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Fibre ID \t= %d\n",fibre_ptr->fibreID);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Nodes \t= %d\n",fibre_ptr->nodesOnFibre);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Radius \t= %g\n",fibre_ptr->radius);CHKERRQ(ierr);

    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Nodes: ");CHKERRQ(ierr);
    int i;
    for (i = 0; i < fibre_ptr->nodesOnFibre; i++)
    {
        Node *node_ptr = fibre_ptr->nodesOnFibreList[i];
        ierr = PetscPrintf(PETSC_COMM_WORLD,"%d, ",node_ptr->nodeID);CHKERRQ(ierr);
    }
    ierr = PetscPrintf(PETSC_COMM_WORLD,"\n");CHKERRQ(ierr);

    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] E^stre \t= %g\n",fibre_ptr->fibreStreEnergy);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] E^bend \t= %g\n",fibre_ptr->fibreBendEnergy);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] E^affn \t= %g\n",fibre_ptr->fibreAffnEnergy);CHKERRQ(ierr);

    return ierr;
}


PetscErrorCode printNodeInfo(Node *node_ptr)
{
    PetscErrorCode ierr;
    
    ierr = PetscPrintf(PETSC_COMM_WORLD,"NODE INFO\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"---------\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Node ID \t= %d\n",node_ptr->nodeID);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Node type \t= %d\n",node_ptr->nodeType);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Internal ID \t= %d\n",node_ptr->globalID);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] S_xyz \t\t= (%0.8g,%0.8g,%0.16g)\n",node_ptr->xyzCoord[0],
                                                                    node_ptr->xyzCoord[1],
                                                                    node_ptr->xyzCoord[2]);
    CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] U_xyz \t\t= (%0.8g,%0.8g,%0.16g)\n",node_ptr->xyzDisplacement[0],
                                                                    node_ptr->xyzDisplacement[1],
                                                                    node_ptr->xyzDisplacement[2]);
    CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] U_xyz^aff \t= (%0.8g,%0.8g,%0.16g)\n", node_ptr->xyzAffDisplacement[0],
                                                                        node_ptr->xyzAffDisplacement[1],
                                                                        node_ptr->xyzAffDisplacement[2]);
    CHKERRQ(ierr);

    return ierr;
}


/* This function will be replaced with a parameter write out */
PetscErrorCode printAnalysis(Box *box_ptr, Parameters *par_ptr)
{
	PetscErrorCode ierr;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"\n[STATUS]\tGamma \t\t= %g\n", par_ptr->gamma);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tYoungsModulus \t= %g\n", par_ptr->youngsModulus);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tRadius \t\t= %g\n", box_ptr->masterFibreList[0].radius);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tEnergyStre \t= %g\n", par_ptr->energyStre);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tEnergyBend \t= %g\n", par_ptr->energyBend);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tEnergyTotl \t= %g\n", par_ptr->energyTotl);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tEnergyAffn \t= %g\n", par_ptr->energyAffn);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tShearModulus \t= %g\n", par_ptr->shearModulus);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tShearModAffn \t= %g\n\n", par_ptr->shearModAffn);CHKERRQ(ierr);

    //ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tE_Aff-E \t= %g\n", par_ptr->energyAffn-par_ptr->energyTotl);CHKERRQ(ierr);
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tG_Aff-G \t= %g\n", par_ptr->shearModAffn-par_ptr->shearModulus);CHKERRQ(ierr);

    //ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\t(E-E_Aff)/E_Aff \t= %g\n", (par_ptr->energyTotl-par_ptr->energyAffn)/par_ptr->energyAffn);CHKERRQ(ierr);

	return ierr;
}


/* Initiates network write out routine */
PetscErrorCode writeAnalysis(const Box *box_ptr, const Parameters *par_ptr)
{
	PetscErrorCode 	ierr;
	FILE *file_ptr;

	file_ptr = fopen(par_ptr->postSolveResults, "w+");

	fprintf(file_ptr, "Gamma \t\t= %g\n", par_ptr->gamma);
    fprintf(file_ptr, "YoungsModulus \t= %g\n", par_ptr->youngsModulus);
    fprintf(file_ptr, "Radius \t\t= %g\n", box_ptr->masterFibreList[0].radius);
    fprintf(file_ptr, "EnergyStre \t= %g\n", par_ptr->energyStre);
    fprintf(file_ptr, "EnergyBend \t= %g\n", par_ptr->energyBend);
    fprintf(file_ptr, "EnergyTotl \t= %g\n", par_ptr->energyTotl);
    fprintf(file_ptr, "EnergyAffn \t= %g\n", par_ptr->energyAffn);
    fprintf(file_ptr, "ShearModulus \t= %g\n", par_ptr->shearModulus);
    fprintf(file_ptr, "ShearModAffn \t= %g\n", par_ptr->shearModAffn);

	ierr = fclose(file_ptr);

	return ierr;
}


/* Writes box information to file */
PetscErrorCode writeBoxLine(FILE *file_ptr, Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	fprintf(file_ptr, "b ");
	fprintf(file_ptr, "%d ", box_ptr->nodeCount);
	fprintf(file_ptr, "%d ", box_ptr->fibreCount);
	fprintf(file_ptr, "%lf ", box_ptr->xyzDimension[0]);
	fprintf(file_ptr, "%lf ", box_ptr->xyzDimension[1]);
	fprintf(file_ptr, "%lf ", box_ptr->xyzDimension[2]);
	fprintf(file_ptr, "%d ", box_ptr->xyzPeriodic[0]);
	fprintf(file_ptr, "%d ", box_ptr->xyzPeriodic[1]);
	fprintf(file_ptr, "%d\n", box_ptr->xyzPeriodic[2]);

	return ierr;
}


/* Writes fibre information to file */
PetscErrorCode writeFibreLine(FILE *file_ptr, Fibre *fibre_ptr)
{
	PetscErrorCode ierr = 0;

	fprintf(file_ptr, "f ");
	fprintf(file_ptr, "%d ", fibre_ptr->fibreID);
	fprintf(file_ptr, "%lf ", fibre_ptr->radius);

	int nOnFibre;
	for (nOnFibre = 0; nOnFibre < fibre_ptr->nodesOnFibre; nOnFibre++)
	{
		fprintf(file_ptr, "%d ", fibre_ptr->nodesOnFibreList[nOnFibre]->nodeID);
	}
	fprintf(file_ptr, "\n");

	return ierr;
}


/* Writes node information to file */
PetscErrorCode writeNodeLine(FILE *file_ptr, Node *node_ptr)
{
	PetscErrorCode ierr = 0;

	PetscScalar xUpdated = node_ptr->xyzCoord[0] + node_ptr->xyzDisplacement[0];
	PetscScalar yUpdated = node_ptr->xyzCoord[1] + node_ptr->xyzDisplacement[1];
	PetscScalar zUpdated = node_ptr->xyzCoord[2] + node_ptr->xyzDisplacement[2];

	/* cleaner method (eliminates duplication) */
	//PetscScalar s_node[DIMENSION];
	//ierr = updatePositionVec(s_node, node_ptr);CHKERRQ(ierr);
	// change xUpdated, y, z etc. below...

	fprintf(file_ptr, "n ");
	fprintf(file_ptr, "%d ",   node_ptr->nodeID);
	fprintf(file_ptr, "%d ",   node_ptr->nodeType);
	fprintf(file_ptr, "%lf ",  xUpdated);
	fprintf(file_ptr, "%lf ",  yUpdated);
	fprintf(file_ptr, "%lf\n", zUpdated);

	return ierr;
}
