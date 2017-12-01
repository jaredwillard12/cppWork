#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	MPI_Init(&argc, &argv);

	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	double coordAndMass[16];

	double j;

	if (rank==0){
		FILE* fin;
		fin = fopen("gravityInput.txt", "r");
		fscanf(fin, "%lf", &j);
		for (int i = 0; i<16; i+=4){
			fscanf(fin, "%lf,%lf,%lf,%lf", coordAndMass+i, coordAndMass+i+1, coordAndMass+i+2, coordAndMass+i+3);
		}
	}

	MPI_Bcast(&j, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double y[4];
	MPI_Scatter(&coordAndMass, 4, MPI_DOUBLE, &y, 4, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	printf("%d has ", rank);
	for(int i=0; i<4; i++){
		printf("%lf, ", y[i]);
	}
	printf("\n");



	MPI_Finalize();
	return 0;
}
