#include <stdio.h>
#include <unistd.h>
#include "mpi.h"

int compute_pi(long long n){
    int i;
    double w, x, sum, pi;
    
    w = 1.0/n;
    sum = 0.0;
    for (i=1; i<=n; i++) {
        x = w*(i-0.5);
        sum = sum + 4/(1+x*x);
    }
    pi = w*sum;
    return pi;
}

int main(int argc, char* argv[])
{
    long long n=1000000000;
	int rank, size;
	double t_start, t_end, l_time, g_time;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	t_start = MPI_Wtime();
	sleep(2);
	if (rank == 1) compute_pi(n);
	t_end= MPI_Wtime();
	l_time = t_end - t_start;
	g_time = 0;
	MPI_Reduce(&l_time, &g_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (!rank) 
		printf("Local Time is %f, Global Time is %f\n", l_time, g_time);
	MPI_Finalize();
	return 0;
}
