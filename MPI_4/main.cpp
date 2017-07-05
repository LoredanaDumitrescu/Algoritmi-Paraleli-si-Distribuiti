#include "mpi.h"
#include <iostream>
using namespace std;

#define N 6

int main(int argc, char *argv[])
{
	int v[N][N] = {
		{ 0, 2, 5, 999, 999, 999 },
		{ 999, 0, 7, 1, 999, 899 },
		{ 999, 9, 0, 4, 999, 999 },
		{ 999, 999, 999, 0, 3, 999 },
		{ 999, 999, 2, 999, 0, 3 },
		{ 999, 5, 999, 2, 4, 0 }
	};
	int result[N][N];
	int nrP, rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nrP);



	for (int k = 0; k < N; k++)
	{
		for (int i = rank; i < N; i = i + nrP)
		{
			for (int j = 0; j < N; j++)
			{
				if (v[i][j] > v[i][k] + v[k][j])
					v[i][j] = v[i][k] + v[k][j];
			}
		}

		MPI_Reduce(v, result, N*N, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

		if (rank == 0) {
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					v[i][j] = result[i][j];
			MPI_Bcast(v, N*N, MPI_INT, 0, MPI_COMM_WORLD);
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}

	if (rank == 0){
		for (int i = 0; i < N; i++){
			for (int j = 0; j < N; j++)
				cout << v[i][j] << " ";
			cout << "\n";
		}

	}

	MPI_Finalize();


	system("pause");
	return 0;
}
