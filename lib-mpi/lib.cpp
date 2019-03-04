#include "lib.h"

// Generates lower and uppwer limits to be used in the zeta function based on
// number of processes, rank and work to be done
void gen_limits(int& from, int& to, int rank, int nprocs, int n)
{
    int workload = n / nprocs;

    rank == 0 ? from = 1 : from = rank * workload;
    to = (rank + 1) * workload;

    // if we cannot divide the number of computations evenly amongst the processes
    // the last process will pick up the slack and compute the rest
    if (rank == nprocs - 1)
        to += n % nprocs;

    // printf("from %d to %d\n", from, to);
}

// Returns true if n is a power of two. If a number is a power of 2 then in
// binary representation, the count of 1 will be one.
bool power_of_two(int n)
{
  return n != 0 && (n & (n - 1)) == 0;
}

// Parses cli arguments to the program
// initiates MPI setting number of process and rank and finaly returns the
// first argument as a int to the calling function.
// Also sets a start time, to be used in the finalize function to calculate
// running time of the program.
int arg_parser(int argc, char** argv, int& nprocs, int& rank, double& time_start)
{
    if (argc < 2) {
        printf("Requires argument: number of intervals\n");
        return 0;
    }
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (! power_of_two(nprocs)) {
        if (rank == 0)
            printf("Number of proceses: %d, is not a power of two\n", nprocs);

        MPI_Finalize();
        return 0;
    }

    if (rank == 0)
        time_start = MPI_Wtime();

    return atoi(argv[1]);
}

// Finalizes MPI and calculates run time of the program and prints results to stdout.
void finalize(int rank, double pi, double time_start)
{

    if (rank == 0) {
        double error = fabs(M_PI - pi);
        double duration = MPI_Wtime() - time_start;
        printf("pi=%f, error=%e, duration=%es\n", pi, error, duration);
    }

    MPI_Finalize();
}
