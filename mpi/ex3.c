#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "mpi.h"

#define BUF_SZ 64
#define SIZE 4

int MPI_Scatter_cz(const void *sendbuf, int sendcount,
        MPI_Datatype sendtype, void *recvbuf, int recvcount,
        MPI_Datatype recvtype, int root, MPI_Comm comm);

int MPI_Scatter_czi(const void *sendbuf, int sendcount,
        MPI_Datatype sendtype, void *recvbuf, int recvcount,
        MPI_Datatype recvtype, int root, MPI_Comm comm);

int main (int argc, char* argv[])
{
    int N=1000000,i;
    int numtasks, rank, scnt, rcnt, src;
    double t_start , t_end, l_time, g_time;
    float sb[SIZE][SIZE] = {
        {1.0, 2.0, 3.0, 4.0},
        {5.0, 6.0, 7.0, 8.0},
        {9.0, 10.0, 11.0, 12.0},
        {13.0, 14.0, 15.0, 16.0} };
    float rb[SIZE];

    MPI_Comm comm;

    MPI_Init(&argc, &argv);
    comm=MPI_COMM_WORLD;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    if (numtasks==SIZE) {
        src=1;
        scnt=SIZE;
        rcnt=SIZE;
        t_start = MPI_Wtime();
        for (i=0; i<N; i++) {
        //MPI_Scatter(sb, scnt, MPI_FLOAT, rb, rcnt, MPI_FLOAT,
        //MPI_Scatter_cz(sb, scnt, MPI_FLOAT, rb, rcnt, MPI_FLOAT,
        MPI_Scatter_czi(sb, scnt, MPI_FLOAT, rb, rcnt, MPI_FLOAT,
                src, comm);
        }

        //printf("rank %d Results: %f %f %f %f\n",rank,rb[0],
        //        rb[1], rb[2], rb[3]);
        for (i=0; i<SIZE; i++) {
            assert(rb[i]==rank*4+i+1);
        }

        t_end= MPI_Wtime() ;
        l_time = t_end - t_start ;
        g_time = 0;
        MPI_Reduce(&l_time, &g_time, 1, MPI_DOUBLE, MPI_MAX, 0,
                comm) ;
        if (!rank)
        printf ("Local Time is %f, Global Time is %f\n", l_time, g_time) ;
    }
    else {
        printf("Must specify %d processors. Terminating.\n",
                SIZE);
    }
    MPI_Finalize();
    return 0;
}

int MPI_Scatter_cz(const void *sendbuf, int sendcount,
        MPI_Datatype sendtype, void *recvbuf, int recvcount,
        MPI_Datatype recvtype, int root, MPI_Comm comm) {
    int size, rank, i;
    MPI_Aint lb, extent;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    MPI_Type_get_extent(sendtype, &lb, &extent);

    if (rank==root) {
        for (i=0; i<size; i++) {
            if (i == root) {
                memcpy(recvbuf, sendbuf+i*sendcount*extent, sendcount*extent);
            }else {
            MPI_Send(sendbuf+i*sendcount*extent,
                    sendcount, sendtype, i, 1,
                    comm);
            }
        }
    }
    if (rank != root) {
    MPI_Recv(recvbuf, recvcount, recvtype, root, 1,
            comm, MPI_STATUS_IGNORE);
    }
}

int MPI_Scatter_czi(const void *sendbuf, int sendcount,
        MPI_Datatype sendtype, void *recvbuf, int recvcount,
        MPI_Datatype recvtype, int root, MPI_Comm comm) {
    int size, rank, i;
    MPI_Aint lb, extent;
    MPI_Request request;
    MPI_Status status;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    MPI_Type_get_extent(sendtype, &lb, &extent);

    if (rank==root) {
        for (i=0; i<size; i++) {
            if (i == root) {
                memcpy(recvbuf, sendbuf+i*sendcount*extent, sendcount*extent);
            }else {
            MPI_Isend(sendbuf+i*sendcount*extent,
                    sendcount, sendtype, i, 1,
                    comm, &request);
            }
            MPI_Wait(&request, &status);
        }
    }
    if (rank != root) {
    MPI_Irecv(recvbuf, recvcount, recvtype, root, 1,
            comm, &request);
    MPI_Wait(&request, &status);
    }
}
