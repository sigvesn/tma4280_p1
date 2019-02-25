#include <lib.h>

double mach(int rank, int nprocs, int n, double x)
{
    double s = 0.0;

    int from, to;
    gen_limits(from, to, rank, nprocs, n);

    for (int i = from; i < to; ++i)
        s += pow(-1, i - 1) * (pow(x, 2 * i - 1) / (2 * i - 1));

    return s;
}

double mach_pi(double a, double b)
{
    return (4 * a - b) * 4;
}

int main(int argc, char** argv)
{

    int nprocs, rank;
    double time_start;
    const int n = arg_parser(argc, argv, nprocs, rank, time_start);
    if (rank == 0)
        printf("Parallel Mach approx of pi with n = %d\n", n);

    double mach_0 = mach(rank, nprocs, n, 1 / 5.0);
    double mach_1 = mach(rank, nprocs, n, 1 / 239.0);

    double pi_0;
    double pi_1;
    MPI_Reduce(&mach_0, &pi_0, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mach_1, &pi_1, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    double pi = mach_pi(pi_0, pi_1);

    finalize(rank, pi, time_start);
}
