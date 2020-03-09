#include "networkWrite.h"

/* Initiates network write out routine */
PetscErrorCode networkWrite(const char *fileName, const char *fileNameAdv, const Box *box_ptr)
{
    PetscErrorCode  ierr;
    PetscBool       advWrite = PETSC_TRUE;
    FILE            *file_ptr;

    ierr = PetscOptionsGetBool(NULL,NULL,"-write_energy",&advWrite,NULL);CHKERRQ(ierr);

    file_ptr = fopen(fileName, "w+");
    ierr = networkStandardWrite(file_ptr, box_ptr);
    ierr = fclose(file_ptr);

    /* when energy write flag is given perform additional write */
    if (advWrite)
    {
        /* write out standard info followed by fibre energy info */
        file_ptr = fopen(fileNameAdv, "w+");
        ierr = networkStandardWrite(file_ptr, box_ptr);
        ierr = networkAdvancedWrite(file_ptr, box_ptr);
        ierr = fclose(file_ptr);
    }

    return ierr;
}


/* \brief Writes out all the standard network info to an opened file */
PetscErrorCode networkStandardWrite(FILE *file_ptr, const Box *box_ptr)
{
    PetscErrorCode  ierr;
    PetscInt        fIndex, nIndex, cIndex;

    /* first written line should be box line */
    ierr = writeBoxLine(file_ptr, box_ptr);CHKERRQ(ierr);

    /* followed by a line for every fibre */
    for (fIndex = 0; fIndex < box_ptr->fibreCount; fIndex++)
    {
        ierr = writeFibreLine(file_ptr, &(box_ptr->masterFibreList[fIndex]));CHKERRQ(ierr);
    }

    /* followed by a line for every node */
    for (nIndex = 0; nIndex < box_ptr->nodeCount; nIndex++)
    {
        ierr = writeNodeLine(file_ptr, &(box_ptr->masterNodeList[nIndex]));CHKERRQ(ierr);
    }

    /* followed by couple lines if the system is coupled */
    for (cIndex = 0; cIndex < box_ptr->coupleCount; cIndex++)
    {
        /* write out couples */
        ierr = writeCoupleLine(file_ptr, &(box_ptr->masterCoupleList[cIndex]));CHKERRQ(ierr);
    }
    
    return ierr;
}


/* Writes out all the additional network energy info to an opened file */
PetscErrorCode networkAdvancedWrite(FILE *file_ptr, const Box *box_ptr)
{
    PetscErrorCode  ierr;
    PetscInt        fIndex;

    /* each fibre should have 4 additional energies written to file */
    for (fIndex = 0; fIndex < box_ptr->fibreCount; fIndex++)
    {
        ierr = writeEnergyLine(file_ptr, &(box_ptr->masterFibreList[fIndex]));CHKERRQ(ierr);
    }
    
    return ierr;
}


PetscErrorCode printFibreInfo(const Fibre *fibre_ptr)
{
    PetscErrorCode ierr;

    ierr = PetscPrintf(PETSC_COMM_WORLD,"FIBRE INFO\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"----------\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Fibre ID \t= %d\n", fibre_ptr->fibreID);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Nodes \t= %d\n",    fibre_ptr->nodesOnFibre);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Radius \t= %g\n",   fibre_ptr->radius);CHKERRQ(ierr);

    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Nodes: ");CHKERRQ(ierr);
    int i;
    for (i = 0; i < fibre_ptr->nodesOnFibre; i++)
    {
        const Node *node_ptr = fibre_ptr->nodesOnFibreList[i];
        ierr = PetscPrintf(PETSC_COMM_WORLD,"%d, ",node_ptr->nodeID);CHKERRQ(ierr);
    }
    ierr = PetscPrintf(PETSC_COMM_WORLD,"\n");CHKERRQ(ierr);

    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] E^stre \t= %g\n",   fibre_ptr->fibreStreEnergy);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] E^bend \t= %g\n",   fibre_ptr->fibreBendEnergy);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] E^affn \t= %g\n",   fibre_ptr->fibreAffnEnergy);CHKERRQ(ierr);

    return ierr;
}


PetscErrorCode printNodeInfo(const Node *node_ptr)
{
    PetscErrorCode ierr;

    ierr = PetscPrintf(PETSC_COMM_WORLD,"NODE INFO\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"---------\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Node ID \t= %d\n",      node_ptr->nodeID);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Node type \t= %d\n",    node_ptr->nodeType);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] Internal ID \t= %d\n",  node_ptr->globalID);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] S_xyz \t\t= (%0.8g,%0.8g,%0.16g)\n",
                        node_ptr->xyzCoord[0], node_ptr->xyzCoord[1], node_ptr->xyzCoord[2]);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] U_xyz \t\t= (%0.8g,%0.8g,%0.16g)\n",
                        node_ptr->xyzDisplacement[0], node_ptr->xyzDisplacement[1], node_ptr->xyzDisplacement[2]);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[INFO] U_xyz^aff \t= (%0.8g,%0.8g,%0.16g)\n", 
                        node_ptr->xyzAffDisplacement[0], node_ptr->xyzAffDisplacement[1], node_ptr->xyzAffDisplacement[2]);
    CHKERRQ(ierr);

    return ierr;
}


/* This function will be replaced with a parameter write out */
PetscErrorCode printAnalysis(const Box *box_ptr, const Parameters *par_ptr)
{
    PetscErrorCode ierr;

    ierr = PetscPrintf(PETSC_COMM_WORLD,"\n[STATUS]\tGamma \t\t= %g\n",      par_ptr->gamma);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tYoungsModulus \t= %g\n",  par_ptr->youngsModulus);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tRadius \t\t= %g\n",       box_ptr->masterFibreList[0].radius);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tEnergyStre \t= %.16e\n",     par_ptr->energyStre);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tEnergyBend \t= %.16e\n",     par_ptr->energyBend);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tEnergyTotl \t= %.16e\n",     par_ptr->energyTotl);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tEnergyPsAf \t= %.16e\n",     par_ptr->energyPsAf);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tEnergyAffn \t= %.16e\n",     par_ptr->energyAffn);CHKERRQ(ierr);
    if (box_ptr->sheetCount > 0) 
    {
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tSheetCount \t= %d\n", box_ptr->sheetCount);CHKERRQ(ierr);
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tFibPerSheet \t= %d\n", box_ptr->fibreCountPerSheet);CHKERRQ(ierr);
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tConFibPerPair\t= %d\n", box_ptr->conFibCountPerSheetPair);CHKERRQ(ierr);
        /*
         * Print out Energy in plane (stretch, bend, total, affine)
         * Print out Energy out of plane (total i.e. stretch and affine)
         */ 
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tInPlnEnergyTotl = %.16e\n", par_ptr->inPlnEnergyTotl);CHKERRQ(ierr);
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tInPlnEnergyAffn = %.16e\n", par_ptr->inPlnEnergyAffn);CHKERRQ(ierr);
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tOutPlnEnergyTot = %.16e\n", par_ptr->outPlnEnergyTotl);CHKERRQ(ierr);
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tOutPlnEnergyAff = %.16e\n", par_ptr->outPlnEnergyAffn);CHKERRQ(ierr);
    }
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tShearModulus \t= %.16e\n",   par_ptr->shearModulus);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tShearModAffn \t= %.16e\n\n", par_ptr->shearModAffn);CHKERRQ(ierr);
    //
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tE_Aff-E \t= %g\n", par_ptr->energyAffn-par_ptr->energyTotl);CHKERRQ(ierr);
    //ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS]\tG_Aff-G \t= %g\n", par_ptr->shearModAffn-par_ptr->shearModulus);CHKERRQ(ierr);

    return ierr;
}


/* Initiates network write out routine */
PetscErrorCode writeAnalysis(const Box *box_ptr, const Parameters *par_ptr)
{
    PetscErrorCode  ierr;
    FILE *file_ptr;

    file_ptr = fopen(par_ptr->postSolveResults, "w+");

    fprintf( file_ptr, "Dimension,");
    fprintf( file_ptr, "Periodicity,");
    fprintf( file_ptr, "TotalNodes," );
    fprintf( file_ptr, "InternalNodes," );
    fprintf( file_ptr, "TotalFibres," );
    fprintf( file_ptr, "TotalCouples," );
    fprintf( file_ptr, "Gamma," );
    fprintf( file_ptr, "YoungsModulus," );
    fprintf( file_ptr, "Radius," );
    fprintf( file_ptr, "EnergyStre," );
    fprintf( file_ptr, "EnergyBend," );
    fprintf( file_ptr, "EnergyTotl," );
    fprintf( file_ptr, "EnergyRatio," );
    fprintf( file_ptr, "EnergyPsAf," );
    fprintf( file_ptr, "EnergyAffn," );
    fprintf( file_ptr, "ShearModulus," );
    fprintf( file_ptr, "ShearModAffn," );
    fprintf( file_ptr, "ShearModRatio\n" );

    fprintf( file_ptr, "(%f;%f;%f),", box_ptr->xyzDimension[0], 
                box_ptr->xyzDimension[1], box_ptr->xyzDimension[2]          );
    fprintf( file_ptr, "(%d;%d;%d),", box_ptr->xyzPeriodic[0], 
                box_ptr->xyzPeriodic[1], box_ptr->xyzPeriodic[2]            );
    fprintf( file_ptr, "%d,",   box_ptr->nodeCount                          );
    fprintf( file_ptr, "%d,",   box_ptr->nodeInternalCount                  );
    fprintf( file_ptr, "%d,",   box_ptr->fibreCount                         );
    fprintf( file_ptr, "%d,",   box_ptr->coupleCount                        );
    fprintf( file_ptr, "%g,",   par_ptr->gamma                              );
    fprintf( file_ptr, "%g,",   par_ptr->youngsModulus                      );
    fprintf( file_ptr, "%g,",   box_ptr->masterFibreList[0].radius          );
    fprintf( file_ptr, "%g,",   par_ptr->energyStre                         );
    fprintf( file_ptr, "%g,",   par_ptr->energyBend                         );
    fprintf( file_ptr, "%g,",   par_ptr->energyTotl                         );
    fprintf( file_ptr, "%g,",   par_ptr->energyBend/par_ptr->energyTotl     );
    fprintf( file_ptr, "%g,",   par_ptr->energyPsAf                         );
    fprintf( file_ptr, "%g,",   par_ptr->energyAffn                         );
    fprintf( file_ptr, "%g,",   par_ptr->shearModulus                       );
    fprintf( file_ptr, "%g,",   par_ptr->shearModAffn                       );
    fprintf( file_ptr, "%g\n",  par_ptr->shearModulus/par_ptr->shearModAffn );

    ierr = fclose(file_ptr);

    return ierr;
}


/* Writes box information to file */
PetscErrorCode writeBoxLine(FILE *file_ptr, const Box *box_ptr)
{
    PetscErrorCode ierr = 0;

    fprintf(file_ptr, "b ");
    fprintf(file_ptr, "%d ",    box_ptr->nodeCount);
    fprintf(file_ptr, "%d ",    box_ptr->fibreCount);
    fprintf(file_ptr, "%lf ",   box_ptr->xyzDimension[0]);
    fprintf(file_ptr, "%lf ",   box_ptr->xyzDimension[1]);
    fprintf(file_ptr, "%lf ",   box_ptr->xyzDimension[2]);
    fprintf(file_ptr, "%d ",    box_ptr->xyzPeriodic[0]);
    fprintf(file_ptr, "%d ",    box_ptr->xyzPeriodic[1]);
    fprintf(file_ptr, "%d\n",   box_ptr->xyzPeriodic[2]);

    return ierr;
}


/* Writes fibre information to file */
PetscErrorCode writeFibreLine(FILE *file_ptr, const Fibre *fibre_ptr)
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
PetscErrorCode writeNodeLine(FILE *file_ptr, const Node *node_ptr)
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


/* Writes couple information to file */
PetscErrorCode writeCoupleLine(FILE *file_ptr, const Couple *cpl_ptr)
{
    PetscErrorCode ierr = 0;

    fprintf(file_ptr, "c ");
    
    int nOnCouple;
    for (nOnCouple = 0; nOnCouple < cpl_ptr->nodesInCouple; nOnCouple++)
    {
        fprintf(file_ptr, "%d ", cpl_ptr->nodeID[nOnCouple]);
    }
    fprintf(file_ptr, "\n");

    return ierr;
}


/* Writes fibre energy information to file */
PetscErrorCode writeEnergyLine(FILE *file_ptr, const Fibre *fibre_ptr)
{
    PetscErrorCode ierr = 0;

    fprintf(file_ptr, "e ");
    fprintf(file_ptr, "%d ",    fibre_ptr->fibreID);
    fprintf(file_ptr, "%g ",   fibre_ptr->fibreStreEnergy);
    fprintf(file_ptr, "%g ",   fibre_ptr->fibreBendEnergy);
    fprintf(file_ptr, "%g ",   fibre_ptr->fibrePsAfEnergy);
    fprintf(file_ptr, "%g\n",  fibre_ptr->fibreAffnEnergy);

    return ierr;
}


