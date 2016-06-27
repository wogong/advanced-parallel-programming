#include <stdio.h>
#include <unistd.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
	int rank, size;
	double t_start, t_end, l_time, g_time;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	t_start = MPI_Wtime();
	sleep(2);
	if (rank == 1) sleep(1);
	t_end= MPI_Wtime();
	l_time = t_end - t_start;
	g_time = 0;
	MPI_Reduce(&l_time, &g_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (!rank) 
		printf("Local Time is %f, Global Time is %f\n", l_time, g_time);
	MPI_Finalize();
	return 0;
}
