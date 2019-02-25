#include <lib.h>

double zeta(int rank, int nprocs, int n)
{
    double s = 0.0;

    int from, to;
    gen_limits(from, to, rank, nprocs, n);

    for (int i = from; i < to; ++i)
        s += 1 / pow(i, 2);

    return s;
}

double zeta_pi(double pi)
{
    return sqrt(pi * 6);
}

int main(int argc, char** argv)
{

    int nprocs, rank;
    double time_start;
    const int n = arg_parser(argc, argv, nprocs, rank, time_start);
    if (rank == 0)
        printf("Parallel Zeta approx of pi with n = %d\n", n);

    double my_pi = zeta(rank, nprocs, n);

    double pi;
    MPI_Reduce(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    pi = zeta_pi(pi);

    finalize(rank, pi, time_start);
}
