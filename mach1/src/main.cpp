#include <lib.h>

// calculates one iteration of the Machin formula for i on value x
double mach(int i, double x)
{
    return pow(-1, i - 1) * (pow(x, 2 * i - 1) / (2 * i - 1));
}

// estimates pi based on getting an appoximation of arctan(1/5) and
// arctan(1/239)
double mach_pi(double a, double b)
{
    return (4 * a - b) * 4;
}

// Mpi implementation of mach with rank 0 as a data distributor.
// relies on functions in ../lib-mpi
int main(int argc, char** argv)
{

    int nprocs, rank, from, to;
    double time_start;
    const int n = arg_parser(argc, argv, nprocs, rank, time_start);

    // We use rank to distrubute data, giving us one less process to compute
    const int workers = nprocs - 1;
    // Elements each process should process
    const int workload = n / workers;

    // vectors and sums for the two values of arctan we need to approximate
    // We reuse the same vector for calulating the elements on the root
    // process, sending data to be summarized when it gets ready
    double vec_0[workload], vec_1[workload];
    double sum_0 = 0, sum_1 = 0, total_sum_0 = 0, total_sum_1 = 0;

    if (rank == 0) {
        printf("Parallel Mach approx of pi with n = %d\n", n);
        printf("running with %d processes\n", nprocs);

        for (int i = 0; i < workers; ++i) {
            gen_limits(from, to, i, workers, n);

            // calculates the vector values
            for (int j = from; j < to; ++j) {
                vec_0[j - workload * i] = mach(j, 1 / 5.0);
                vec_1[j - workload * i] = mach(j, 1 / 239.0);
            }

            // when the vectors are filled, they are ready to be sendt for summation
            MPI_Send(vec_0, workload, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD);
            MPI_Send(vec_1, workload, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD);
        }
    } else {
        // All other processes receiving vectors (or part of the total vectors) and summing it

        MPI_Recv(vec_0, workload, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(vec_1, workload, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < workload; ++i) {
            sum_0 += vec_0[i];
            sum_1 += vec_1[i];
        }
    }

    // Finaly we reduce all the individual summations to a total sum in rank 0
    MPI_Reduce(&sum_0, &total_sum_0, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sum_1, &total_sum_1, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // This sum is now the approximation of arctan we need to estimate pi
    double pi = mach_pi(total_sum_0, total_sum_1);

    finalize(rank, pi, time_start);
}
