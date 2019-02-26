#include <lib.h>
#include <omp.h>

// calculates the Riemann zeta function from 1 to n on value 2
double zeta(int n)
{
    double s = 0.0;

// We use the parallel for directive, allowing omp to split the loops accross
// our avaliable processors. We reduce into the sum variable 's', allowing s to
// be private in the loop and finaly summarized before returning as a final
// result
#pragma omp parallel for schedule(static) reduction(+ : s)
    for (int i = 1; i < n; ++i)
        s += 1 / pow(i, 2);

    return s;
}

// estimates pi by approximating the zeta function from n->inf
double zeta_pi(int n)
{
    return sqrt(zeta(n) * 6);
}

// Parallel shared memory implementation of zeta
// relies on functions in ../lib
int main(int argc, char** argv)
{

    char name[] = "Parallel OMP Zeta";
    double time_start;
    const int n = arg_parser(argc, argv, zeta_pi, name, time_start);
    if (n <= 0)
        return 0;

    printf("%s approx of pi with n = %d\n", name, n);

    double pi = zeta_pi(n);

    finalize(pi, time_start);
}
