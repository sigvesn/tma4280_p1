#include <lib.h>

double zeta(int i)
{
    return 1 / pow(i, 2);
}

double zeta_pi(double n)
{
    return sqrt(n * 6);
}

int main(int argc, char** argv)
{

    int nprocs, rank, from, to;
    double time_start;
    const int n = arg_parser(argc, argv, nprocs, rank, time_start);

    const int workers = nprocs - 1;
    const int workload = n / workers;

    double vec[workload];
    double sum = 0, total_sum = 0;

    if (rank == 0) {
        printf("Parallel Zeta approx of pi with n = %d\n", n);

        for (int i = 0; i < workers; ++i) {
            gen_limits(from, to, i, workers, n);

            for (int j = from; j < to; ++j)
                vec[j - workload * i] = zeta(j);

            MPI_Send(vec, workload, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(vec, workload, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < workload; ++i)
            sum += vec[i];
    }

    MPI_Reduce(&sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double pi = zeta_pi(total_sum);

    finalize(rank, pi, time_start);
}
