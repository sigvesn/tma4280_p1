#include <lib.h>

// calculates iterations of the Machin formula on value x for a part of the
// total sum, based on number of processes running
double mach(int rank, int nprocs, int n, double x)
{
    double s = 0.0;

    int from, to;
    gen_limits(from, to, rank, nprocs, n);

    for (int i = from; i < to; ++i)
        s += pow(-1, i - 1) * (pow(x, 2 * i - 1) / (2 * i - 1));

    return s;
}

// estimates pi based on getting an appoximation of arctan(1/5) and
// arctan(1/239)
double mach_pi(double a, double b)
{
    return (4 * a - b) * 4;
}

// Mpi implementation of mach dividing data equally amongst the processes,
// which computes a partial sum of the data
// relies on functions in ../lib-mpi
int main(int argc, char** argv)
{

    int nprocs, rank;
    double time_start;
    const int n = arg_parser(argc, argv, nprocs, rank, time_start);

    if (rank == 0) {
        printf("Parallel Mach approx of pi with n = %d\n", n);
        printf("running with %d processes\n", nprocs);
    }
        

    // each rank computes a part of mach
    double mach_0 = mach(rank, nprocs, n, 1 / 5.0);
    double mach_1 = mach(rank, nprocs, n, 1 / 239.0);

    // reduces all the sums to a total sum in rank 0
    double pi_0, pi_1;
    MPI_Reduce(&mach_0, &pi_0, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mach_1, &pi_1, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // This sum is now the approximation of arctan we need to estimate pi
    double pi = mach_pi(pi_0, pi_1);

    finalize(rank, pi, time_start);
}
