 #include <time.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include<cuda.h>



 #define MAX_THREADS	4

 #define N		30

 int*	r_values;
 int*	d_values;

 // initializare date
 void Init(int* values, int i)
 {
	srand( time(NULL) );
        printf("\n------------------------------\n");

        if (i == 0)
        {
        // Distributia uniforma
                printf("Data set distribution: Uniform\n");
                for (int x = 0; x < N; ++x)
                {
                        values[x] = rand() % 100;
                        printf("%d ", values[x]);
                }
        }
 }


 // functia Kernel
 __global__ static void quicksort(int* values)

{
 #define MAX_LEVELS	5

	int pivot, L, R;
	int idx =  threadIdx.x + blockIdx.x * blockDim.x;
	int start[MAX_LEVELS];
	int end[MAX_LEVELS];

	start[idx] = idx;
	end[idx] = N - 1;
	while (idx >= 0) {
		L = start[idx];
		R = end[idx];
		if (L < R) {
			pivot = values[L];
			while (L < R)
                        {
				while (values[R] >= pivot && L < R)
					R--;
				if(L < R)
					values[L++] = values[R];
				while (values[L] < pivot && L < R)
					L++;
				if (L < R)
					values[R--] = values[L];
			}
			values[L] = pivot;
			start[idx + 1] = L + 1;
			end[idx + 1] = end[idx];
			end[idx++] = L;
			if (end[idx] - start[idx] > end[idx - 1] - start[idx - 1])
                               {
	                        // swap start[idx] and start[idx-1]
        	                int tmp = start[idx];
                	        start[idx] = start[idx - 1];
                        	start[idx - 1] = tmp;

	                        // swap end[idx] and end[idx-1]
        	                tmp = end[idx];
                	        end[idx] = end[idx - 1];
                        	end[idx - 1] = tmp;
	                       }

		}
		else
			idx--;
	}
}

 int main(int argc, char **argv)
 {
    double start,end;

 	size_t size = N * sizeof(int);

 	// aloca memoria gazda
 	r_values = (int*)malloc(size);

	// aloca memorie pe dispozitiv
    cudaMalloc((void**)&d_values, size) ;

	// aloca thread-uri
    const unsigned int cThreadsPerBlock = 4;


       int i=0;
                // initializeaza datele
        Init(r_values, i);

	 	// copiaza datele pe dispozitiv
		cudaMemcpy(d_values, r_values, size, cudaMemcpyHostToDevice) ;

		printf("Beginning kernel execution...\n");


start=timerval();


		// execute kernel
 		quicksort <<< MAX_THREADS / cThreadsPerBlock, MAX_THREADS / cThreadsPerBlock >>> (d_values);


end=timerval();




		 cudaMemcpy(r_values, d_values, size, cudaMemcpyDeviceToHost) ;

	 	// test print
 		for (int i = 0; i < N; i++)
       		{
 			printf("%d ", r_values[i]);
 		}
 		printf("\n");
		printf("%lf",end-start);


		// test
                printf("\nTesting results...\n");
                for (int x = 0; x < N - 1; x++)
                {
                        if (r_values[x] > r_values[x + 1])
                        {
                                printf("Sorting failed.\n");
                                break;
                        }
                        else
                                if (x == N - 2)
                                        printf("SORTING SUCCESSFUL\n");
                }



 	// elibereaza memoria
    cudaFree(d_values) ;
 	free(r_values);


 	cudaThreadExit();
}
