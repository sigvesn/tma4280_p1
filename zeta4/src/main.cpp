#include <lib.h>
#include <omp.h>

// calculates iterations of the Riemann zeta gunction on s=2 for a part of the
// total sum, based on number of processes running
double zeta(int rank, int nprocs, int n)
{
    double s = 0.0;

    int from, to;
    gen_limits(from, to, rank, nprocs, n);

// We use the parallel for directive, allowing omp to split the loops accross
// our avaliable processors. We reduce into the sum variable 's', allowing s to
// be private in the loop and finaly summarized before returning as a final
// result

#pragma omp parallel for schedule(static) reduction(+ : s)
    for (int i = from; i < to; ++i)
        s += 1 / pow(i, 2);

    return s;
}

// estimates pi based on getting an appoximation of of zeta
double zeta_pi(double pi)
{
    return sqrt(pi * 6);
}

// Mpi implementation of zeta dividing data equally amongst the processes, with
// further division of work amongst the processors of each process which, which
// computes a partial sum of the data
// relies on functions in ../lib-mpi
int main(int argc, char** argv)
{

    int nprocs, rank;
    double time_start;
    const int n = arg_parser(argc, argv, nprocs, rank, time_start);
    if (rank == 0)
        printf("Parallel Zeta OMP approx of pi with n = %d\n", n);

    double my_pi = zeta(rank, nprocs, n);

    double pi;
    MPI_Reduce(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    pi = zeta_pi(pi);

    finalize(rank, pi, time_start);
}
