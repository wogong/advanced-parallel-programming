#include "mpi.h"
#include <stdio.h>
#include <assert.h>
#define BUF_SZ 64

int main (int argc, char* argv[])
{
    int rank, size, i;
    double t_start, t_end, l_time, g_time;
    int data[BUF_SZ];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (!rank) {
        for (i=0; i<BUF_SZ; i++)
            data[i] = i+1;
    }

    t_start = MPI_Wtime();
    if (!rank) {
        for (i=1; i<size; i++)
            MPI_Send(data, BUF_SZ, MPI_INT, i, 123, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(data, BUF_SZ, MPI_INT, 0, 123, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    t_end = MPI_Wtime();

    for (i=0; i<BUF_SZ; i++) {
        assert(data[i]==i+1);
    }
    l_time = t_end -t_start;
    g_time = 0;
    MPI_Reduce(&l_time, &g_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (!rank) {
        printf("Local time is %f, global time is %f\n", l_time, g_time);
    }
    MPI_Finalize();

    return 0;
}
