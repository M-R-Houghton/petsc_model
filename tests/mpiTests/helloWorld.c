/*
	A "Hello World" program for MPI.
*/

#include <mpi.h>	   // Include the MPI library
#include <stdio.h>	   // For stdout
#include <stdlib.h>

int main( int argc, char **argv )
{
	int numprocs, rank;
	
	// Initialise MPI; must be first MPI call
	MPI_Init( &argc, &argv );

	// Size of world communicator, i.e. the total number of processes (should equal the -n argument of mpiexec when launched)
	MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

	// Our rank within the world communicator; range 0 to numprocs-1 inclusive.
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );

	// Send rank to stdout
	printf( "Rank %i of %i\n", rank, numprocs );

	// Finalise MPI; must be last MPI call
	MPI_Finalize();

	return EXIT_SUCCESS;
}