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

	fclose(file_ptr);

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

    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tE_Aff-E \t= %g\n", par_ptr->energyAffn-par_ptr->energyTotl);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tG_Aff-G \t= %g\n", par_ptr->shearModAffn-par_ptr->shearModulus);CHKERRQ(ierr);

    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\t(E-E_Aff)/E_Aff \t= %g\n", (par_ptr->energyTotl-par_ptr->energyAffn)/par_ptr->energyAffn);CHKERRQ(ierr);

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