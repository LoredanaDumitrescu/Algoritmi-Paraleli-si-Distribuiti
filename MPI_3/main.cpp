#include "mpi.h"
#include <time.h>
#include <iostream>


using namespace std;

#define SIZE 12
#define NR 0

int main(int argc, char *argv[])
{
	int rank, nrP, slice, indexVpoz;
	int v[SIZE], segment[SIZE], Vpoz[SIZE], finalFound[SIZE];
	bool display = false;

	srand((unsigned int)time(NULL));

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nrP);


	//imparte in segmente egale
	slice = SIZE / nrP;

	//segmentele trebuie sa fie egale
	if (SIZE % nrP != 0)
		++slice;
	//blocheaza procesul curent pana cand toate celelalte procese din communicator isi termina rutina
	MPI_Barrier(MPI_COMM_WORLD);

	//initializeaza vectorul
	if (rank == 0)
	{
		cout << "Elementele vectorului:\n";
		for (int i = 0; i < SIZE; i++)
		{
			v[i] = rand() % 5;
			cout << v[i] << " ";
			finalFound[i] = -1;
		}

	}
	//trimite segmente egale  de la root catre celelalte procese din communicator
	MPI_Scatter(v, slice, MPI_INT, segment, slice, MPI_INT, 0, MPI_COMM_WORLD);
	cout << "\n\nRank: " << rank << ". Segement size: " << slice << "\n";
	//initializare
	for (int i = 0; i < slice; i++)
		Vpoz[i] = -1;

	indexVpoz = 0;
	for (int i = 0; i < slice; i++)
	{
		if (segment[i] == NR)
		{
			Vpoz[++indexVpoz] = i + rank * slice;
		}
	}
	//reuneste toate pozitiile in vectorul finalFound prin intermediul Gather
	MPI_Gather(Vpoz, slice, MPI_INT, finalFound, slice, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int i = 0; i < SIZE ; i++)
		{
			if (finalFound[i] >= 0)
			{
				if (!display)
					cout << "\n\nNumarul a fost gasit pe urmatoatele pozitii: \n";
				display = true;
				cout << finalFound[i] << " ";
			}
		}
		if (!display)
			cout << "\n\n Numarul nu a fost gasit.";
	}

	MPI_Finalize();
	cout << endl;
	system("pause");
	return 0;
}
