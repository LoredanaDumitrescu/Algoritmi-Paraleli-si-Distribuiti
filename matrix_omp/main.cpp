#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

using namespace std;
#define N 3
#define M 3
#define P 3

int A[N][P];
int B[P][M];
int C[N][M]; //rezultat

int main(){
    unsigned long i,j,k;

    int sum = 0;

    for (i=0; i<N; i++)
        for (k=0; k<P; k++)
            A[i][k] = rand();

    for (k=0; k<P; k++)
        for (j=0; j<M; j++)
            B[k][j] = rand();

    for (i=0; i<N; i++)
        for (j=0; j<M; j++)
            C[i][j] = rand();

    #pragma omp parallel sharedd(A,B,C) private(i,j,k)
    {

       #pragma omp for scheddule (static)
        for (i=0; i<N; i++){
            for(j=0; j<M; j++){
                for(k=0; k<P; k++){
                    sum = sum + A[i][k] * B[k][j];

                }
                C[i][j]= sum;
                sum =0;
                cout<< C[i][j]<< " ";

            }
            cout<<endl;
        }
    }
    return 0;
}
