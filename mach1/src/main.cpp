#include <lib.h>

double mach(int i, double x)
{
    return pow(-1, i - 1) * (pow(x, 2 * i - 1) / (2 * i - 1));
}

double mach_pi(double a, double b)
{
    return (4 * a - b) * 4;
}

int main(int argc, char** argv)
{

    int nprocs, rank, from, to;
    double time_start;
    const int n = arg_parser(argc, argv, nprocs, rank, time_start);

    const int workers = nprocs - 1;
    const int workload = n / workers;

    double vec_0[workload], vec_1[workload];
    double sum_0 = 0, sum_1 = 0, total_sum_0 = 0, total_sum_1 = 0;

    if (rank == 0) {
        printf("Parallel Mach approx of pi with n = %d\n", n);

        for (int i = 0; i < workers; ++i) {
            gen_limits(from, to, i, workers, n);

            for (int j = from; j < to; ++j) {
                vec_0[j - workload * i] = mach(j, 1 / 5.0);
                vec_1[j - workload * i] = mach(j, 1 / 239.0);
            }

            MPI_Send(vec_0, workload, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD);
            MPI_Send(vec_1, workload, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD);
        }

    } else {
        MPI_Recv(vec_0, workload, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(vec_1, workload, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < workload; ++i) {
            sum_0 += vec_0[i];
            sum_1 += vec_1[i];
        }
    }

    MPI_Reduce(&sum_0, &total_sum_0, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sum_1, &total_sum_1, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double pi = mach_pi(total_sum_0, total_sum_1);

    finalize(rank, pi, time_start);
}
