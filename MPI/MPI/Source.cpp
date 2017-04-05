#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 1000

int rank, size;
double start_time;
double end_time;

int y, x, i, port1, port2, port3;
int check = 0;         // verifica daca numarul este prim
int signal = 0;       // verifica daca se fac procesele slave

MPI_Status status;
MPI_Request request;

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv); //initializare MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // obtine rank-ul
	MPI_Comm_size(MPI_COMM_WORLD, &size); //obtine numarul de procese

	if (rank == 0) {                  
		start_time = MPI_Wtime();
		printf("2\n");                  //afiseaza numarul 2 deoarece acest algoritm functioneaza doar pentru numere impare
		port1 = (N / size );           // imparte N pentru fiecare proces

		for (i = 1; i<size - 1; i++) {            // procesul master trimite cate o portiune fiecarui proces slave  
			port2 = port1 * i;              // limita inferioara pentru procesul i
			port3 = ((i + 1)*port1) - 1;        //  limita superioara pentru procesul i
			MPI_Isend(&port2, 1, MPI_INT, i, 100, MPI_COMM_WORLD, &request);
			MPI_Isend(&port3, 1, MPI_INT, i, 101, MPI_COMM_WORLD, &request);
		}

		port2 = (size - 1)*port1; 
		port3 = N;     // ultimul proces ia portiunea ramasa
		MPI_Isend(&port2, 1, MPI_INT, (size - 1), 100, MPI_COMM_WORLD, &request);
		MPI_Isend(&port3, 1, MPI_INT, (size - 1), 101, MPI_COMM_WORLD, &request);

		for (x = 3; x < port1; x = x + 2) {   //procesul master
			check = 0;
			for (y = 3; y <= x / 2; y = y + 2) {
				if (x%y == 0)
				{ 
					check = 1;
					break;
				}
			}
			if (check == 0) printf("%d\n", x);
		}
	}

	if (rank > 0) {                    // procesele slave

		MPI_Recv(&port2, 1, MPI_INT, 0, 100, MPI_COMM_WORLD, &status);
		MPI_Recv(&port3, 1, MPI_INT, 0, 101, MPI_COMM_WORLD, &status);
		if (port2 % 2 == 0)
			port2++;                                        
		for (x = port2; x <= port3; x = x + 2) {
			check = 0;
			for (y = 3; y <= x / 2; y = y + 2) {
				if (x%y == 0)
				{
					check = 1;
					break;
				}
			}
			if (check == 0)
				printf("%d\n", x);
		}
		signal = rank;
		MPI_Isend(&signal, 1, MPI_INT, 0, 103, MPI_COMM_WORLD, &request);  // informeaza procesul master ca treaba a fost terminata

	}

	if (rank == 0) {                                                   
		for (i == 1; i < size; i++) {
			MPI_Recv(&signal, 1, MPI_INT, i, 103, MPI_COMM_WORLD, &status);  
		}
		end_time = MPI_Wtime();
		printf("\nRunning Time = %f \n\n", end_time - start_time);
	}
	MPI_Finalize();
	return 0;
}