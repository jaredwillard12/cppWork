#include <cstdio>
#include <mpi.h>

#define N 10

#define _index(i,j) i*N+j

inline double T(double *plate, int i, int j){
	return 0.25 * (plate[_index(i-1,j)] + 
			plate[_index(i,j-1)] +
			plate[_index(i+1,j)] +
			plate[_index(i, j+1)]);
}

void send_up(double *buff){
	static int rank;
	static int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank!=0){
		MPI_Send(buff, N, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD);
	}
}

void recv_down(double *buff){
	static int rank;
	static int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank!=size-1){
		MPI_Recv(buff, N, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD);
	}
}

void recv_up(double *buff){
	static int rank;
	static int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank!=0){
		MPI_Recv(buff, N, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD);
	}
}

void send_down(double *buff){
	static int rank;
	static int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank!=size-1){
		MPI_Send(buff, N, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD);
	}
}

double relative_diff(double* prev, double* current, int n_rows){
	double max_diff = 0;
	for (int i=1; i< n_rows-1; i++){
		for (int j=0; j<N; j++){
			if (prev[_index(i,j)] == current[_index(i,j)]){
				continue;
			}
			double diff = abs(prev[_index(i,j)] - current[_index(i,j)]);
			diff /= std::max(abs(prev[_index(i,j)], current[_index(i,j)]));
			max_diff = std::max(diff, max_diff);
		}
	}
	return max_diff;
}

int main(int argc, char *argv[]){
	int size, rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int rows = ((N-2)/4) + 2
	std::vector<double> local_data(N*rows, 0);
	std::vector<double> local_data1(N*rows, 0);
	double increment = 90.0/(N-1);
	if (rank == size -1){
		for (int i =0; i<N; i++){
			local_data[_index(rows-1,i)]=increment*i;
		}
	}

	int start_row = rank*(N-2)/size;
	for (int i=start_row; i<start_row+rows; i++){
		local_data[_index(i,N-1)]=increment*i;
	}

	double max_diff=1;
	while (max_diff>1e-4){
		send_up(local_data.data());
		recv_down(local_data.data());
		send_down(local_data.data());
		recv_up(local_data.data());
		for(int i=1; i<rows; i++){
			for(int j=0; j<N; j++){
				local_data1[_index(i,j)] = T(local_data.data(), i, j);
			}
		}
		double diff = relative_diff(local_data.data(), local_data1.data(), rows);
		MPI_Allreduce(&diff, &max_diff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
	}

}
