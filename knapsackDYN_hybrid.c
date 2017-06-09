
/*** 
 * Part 3: MPI-OMP Implementation for 0-1 Knapsack problem 
 * MPI Dynamic Programming based solution 
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <omp.h>
#include <string.h>

// A utility function that returns maximum of two integers
int max(int a, int b) { return (a > b)? a : b; }


// Returns the maximum value that can be put in a knapsack of capacity W
static int knapSack(long int W, long int N, int wt[], int val[], double* timeSpent)
{
	
	long int i, w; 
    int result,size,rank,threads;
    int *K = (int*) malloc((W+1)*sizeof(int));
    int *KTemp = (int*) malloc((W+1)*sizeof(int));
       
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 


    //initial time
	double start=MPI_Wtime();

    if(size > 4)
        threads = 4;
    else 
        threads = size;    
    
    for (i = 0; i <= N; i++)
    { 
        #pragma omp parallel for num_threads(threads)
        for (w = 0; w <= W; w++)
        {
            if (i==0 || w==0)
                K[w] = 0;
            else if (wt[i-1] <= w)
                K[w] = max(val[i-1] + KTemp[w-wt[i-1]], KTemp[w]);
            else
                K[w] = KTemp[w];

        }

        int *tmp = KTemp;
        KTemp = K;
        K = tmp;
    }

    *timeSpent = MPI_Wtime()-start;
    
    MPI_Finalize();
    if (rank==size-1)
        result = K[W];
    else 
        result = 0;
     
    free(K);
    free(KTemp);

    return result;      

}

int main(int argc, char **argv){

    FILE *test_file;
    double start_time, elapsed_time; /* for time measurement */

    int *val, *wt;   // width and cost values
    long int Nitems; // Number of items
    long int Width;  // Max. load to carry
    long int cont;    // counter
    double tpivot1=0,tpivot2=0,tpivot3=0; //time counting
    struct timeval tim;

	if (argc!=2) {
	  printf("\n\nError, mising parameters:\n");
	  printf("format: test_file \n");	 
	  return 1;
	}
    

    //Capture first token time - init execution
    gettimeofday(&tim, NULL);
    tpivot1 = tim.tv_sec+(tim.tv_usec/1000000.0);
    
	if (!(test_file=fopen(argv[1],"r"))) {
	  printf("Error opening Value file: %s\n",argv[1]);
	  return 1;
	}

    //Reading number of items and Maximum width
	fscanf(test_file,"%ld %ld\n",&Nitems, &Width);
    
    //printf("%ld\n",Nitems*sizeof(int));
	double timeSpent=0;
	val = (int *)malloc(Nitems*sizeof(int)); //values for each element
	 wt = (int *)malloc(Nitems*sizeof(int)); //width  for each element
    
    //Reading value and width for each element
	for (cont=0;cont<Nitems;cont++){
	  fscanf(test_file,"%d,%d\n",&val[cont],&wt[cont]);
	}

     //    printf("\n\n ---- Results TCapacity:items:Tcost:toptim:ttime ----\n\n");
    gettimeofday(&tim, NULL);
    tpivot2 = (tim.tv_sec+(tim.tv_usec/1000000.0));
    //printf("HOLA2\n");
    
    //int K[Nitems+1][Width+1] ;
    int result=  knapSack(Width,Nitems, wt, val,&timeSpent);
      // we only print the result for the core with result greater than 0 (core 0).
      if (result>0){
         printf("%ld:%ld:%d", Width, Nitems, result);

    	gettimeofday(&tim, NULL);

    	tpivot3 = (tim.tv_sec+(tim.tv_usec/1000000.0));
   		 printf(":%.6lf:%.6lf\n", timeSpent,tpivot3-tpivot1);
   }

	free(val);
	free(wt);
	
	fclose(test_file);

	return 0;
}
