#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#define N 3
#define n 3
using namespace std;

struct v
{
    int i;
    int j;
};

int A[N][N] = {{1, 2, 4}, {3, 4, 5}, {4, 5, 7}};
int B[N][N] = {{2, 3, 9}, {4, 5, 7}, {2, 3, 9}};
int C[N][N];

static void * multiplication(void *arg){
    struct v *data = (struct v *)arg;

    int l;
    for(l=0; l < N; l++)
    {
        int i=(data[l]).i;
        int j=(data[l]).j;
        double sum=0;
        int d;

        for (d = 0; d < N; d++)
        {
            sum = sum + A[i][d]*B[d][j];
        }

        C[i][j] = sum;
        sum = 0;
    }
    return 0;
}

int main(void)
{
    pthread_t threads[n];
    int i, k;

    struct v **data;
    data = (struct v **)malloc(n * sizeof(struct v*));

    for(i = 0; i < n; i++)
    {
        data[i] = (struct v *)malloc(n * sizeof(struct v));

        for(k = 0; k < n; k++)
        {
            data[i][k].i = i;
            data[i][k].j = k;
        }

        pthread_create(&threads[i], NULL, multiplication, data[i]);
    }

    for(i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (i = 0; i < N; i++)
    {
        for (k = 0; k < N; k++)
        {
            cout<< C[i][k]<<" ";

        }

            cout<<endl;

        free(data[i]);
    }

    free(data);

    return 0;
}
