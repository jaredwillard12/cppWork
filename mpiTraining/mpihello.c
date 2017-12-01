#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int rank, size;
	MPI_Inint(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank)
	MPI_Comm_size(MPI_COMM_WORLD, &size)

	printf("Hello from rank %d\n", rank);

	MPI_Finalize();
	return 0;

}
