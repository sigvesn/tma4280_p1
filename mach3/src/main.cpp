#include "lib.h"
#include <omp.h>

// calculates the Machin formula from 1 to n on value x
double mach(int n, double x)
{
    double s = 0.0;

// We use the parallel for directive, allowing omp to split the loops accross
// our avaliable processors. We reduce into the sum variable 's', allowing s to
// be private in the loop and finaly summarized before returning as a final
// result
#pragma omp parallel for schedule(static) reduction(+ : s)
    for (int i = 1; i < n; ++i)
        s += pow(-1, i - 1) * (pow(x, 2 * i - 1) / (2 * i - 1));

    return s;
}

// estimates pi by approximating arctan using the Machin formula for value 1/5
// and 1/239
double mach_pi(int n)
{
    return (4 * mach(n, 1 / 5.0) - mach(n, 1 / 239.0)) * 4;
}

// Parallel shared memory implementation of mach
// relies on functions in ../lib
int main(int argc, char** argv)
{

    char name[] = "Parallel OMP Mach";
    double time_start;
    const int n = arg_parser(argc, argv, mach_pi, name, time_start);

    if (n <= 0)
        return 0;

    printf("Serial Mach approx of pi with n = %d\n", n);

    double pi = mach_pi(n);

    finalize(pi, time_start);
}
