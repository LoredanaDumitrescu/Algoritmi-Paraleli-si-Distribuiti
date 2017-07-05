#include <iostream>
#include <mpi.h>
#include <math.h>

using namespace std;

#define SIZE 20

int main(int argc, char *argv[])
{
    int  rank;
    int  nrp;   // nr procese
	int v[SIZE]={ 7, 8, 9, 1, 2, 4, 5, 6 };
	int i, x, inceput, sfarsit, rezultat;
	int pozitie=0;
	int cautat=6;


    MPI_Init(&argc, &argv);

    /*Determina numarul de procese asociate comunicatorului */
    MPI_Comm_size(MPI_COMM_WORLD, &nrp);

    /*Determina rankul procesului apelat in comunicator*/
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	MPI_Bcast(v, SIZE, MPI_INT, 0, MPI_COMM_WORLD);

	//adauga o portiune din data
	x = SIZE/nrp; // must be an integer
	inceput = rank * x;
	sfarsit = inceput + x;
	for(i=inceput; i<sfarsit; i++) {
		if(cautat==v[i]){
			pozitie=i;
		}
	}

    //reuneste toate pozitiile in vectorul finalFound prin intermediul MPI_Reeduce
	MPI_Reduce(&poz, &rezultat, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if(0 == rank) {
		if(rezultat==0)
		{
			cout<<"Numarul nu a fost gasit!!\n";
		}
		else{
			cout<<"Pozitia maxima "<< rezultat <<"\n";
		}
	}


    MPI_Finalize();
    system("pause");
    return 0;
}
