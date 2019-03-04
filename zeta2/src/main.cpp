#include <lib.h>

// calculates iterations of the Riemann zeta gunction on s=2 for a part of the
// total sum, based on number of processes running
double zeta(int rank, int nprocs, int n)
{
    double s = 0.0;

    int from, to;
    gen_limits(from, to, rank, nprocs, n);

    for (int i = from; i < to; ++i)
        s += 1 / pow(i, 2);

    return s;
}

// estimates pi based on getting an appoximation of of zeta
double zeta_pi(double pi)
{
    return sqrt(pi * 6);
}

// Mpi implementation of zeta dividing data equally amongst the processes,
// which computes a partial sum of the data
// relies on functions in ../lib-mpi
int main(int argc, char** argv)
{

    int nprocs, rank;
    double time_start;
    const int n = arg_parser(argc, argv, nprocs, rank, time_start);

    if (n <= 0)
        return 0;

    if (rank == 0) {
        printf("Parallel Zeta approx of pi with n = %d\n", n);
        printf("running with %d processes\n", nprocs);
    }

    // each rank computes a part of zeta
    double my_pi = zeta(rank, nprocs, n);

    // reduces all the sums to a total sum in rank 0
    double pi;
    MPI_Reduce(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // This sum is now the approximation of zeta we need to estimate pi
    pi = zeta_pi(pi);

    finalize(rank, pi, time_start);
}
