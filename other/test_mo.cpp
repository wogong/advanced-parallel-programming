#include <cstdlib>
#include <iostream>
#include <ctime>
#include "mpi.h"
#include "omp.h"

using namespace std;

int main ( int argc, char *argv[] );

int main ( int argc, char *argv[] )

{
    int myid;
    int nprocs;
    int this_thread;
    MPI::Init();
    myid=MPI::COMM_WORLD.Get_rank();
    nprocs=MPI::COMM_WORLD.Get_size();
#pragma omp parallel private(this_thread)
    {
        this_thread=omp_get_thread_num();
        cout <<this_thread<<" thread from "<<myid<<" is ok\n";
    }
    MPI::Finalize();
    return 0;
}
