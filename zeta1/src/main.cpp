#include <lib.h>

// calculates one iteration of the Riemann zeta function for i on s=2
double zeta(int i)
{
    return 1 / pow(i, 2);
}

// estimates pi based on getting an appoximation of of zeta
double zeta_pi(double n)
{
    return sqrt(n * 6);
}

int main(int argc, char** argv)
{

    int nprocs, rank, from, to;
    double time_start;
    const int n = arg_parser(argc, argv, nprocs, rank, time_start);

    // We use rank to distrubute data, giving us one less process to compute
    const int workers = nprocs - 1;
    // Elements each process should process
    const int workload = n / workers;

    // vector and sums for the values of the zeta function
    // We reuse the same vector for calulating the elements on the root
    // process, sending data to be summarized when it gets ready
    double vec[workload];
    double sum = 0, total_sum = 0;

    if (rank == 0) {
        printf("Parallel Zeta approx of pi with n = %d\n", n);

        for (int i = 0; i < workers; ++i) {
            gen_limits(from, to, i, workers, n);

            // calculates the vector values
            for (int j = from; j < to; ++j)
                vec[j - workload * i] = zeta(j);

            // when the vector is filled, they are ready to be sendt for summation
            MPI_Send(vec, workload, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD);
        }
    } else {
        // All other processes receiving the vector (or part of the total vector) and summing it

        MPI_Recv(vec, workload, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < workload; ++i)
            sum += vec[i];
    }

    // Finaly we reduce all the individual summations to a total sum in rank 0
    MPI_Reduce(&sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // This sum is now the approximation of zeta we need to estimate pi
    double pi = zeta_pi(total_sum);

    finalize(rank, pi, time_start);
}
