#include "lib.h"
#include <omp.h>

double mach(int n, double x)
{
    double s = 0.0;

#pragma omp parallel for reduction(+ \
                                   : s)
    for (int i = 1; i < n; ++i)
        s += pow(-1, i - 1) * (pow(x, 2 * i - 1) / (2 * i - 1));

    return s;
}

double mach_pi(int n)
{
    return (4 * mach(n, 1 / 5.0) - mach(n, 1 / 239.0)) * 4;
}

int main(int argc, char** argv)
{

    char name[] = "Parallel Mach";
    double time_start;
    const int n = arg_parser(argc, argv, mach_pi, name, time_start);

    if (n <= 0)
        return 0;

    printf("Serial Mach approx of pi with n = %d\n", n);

    double pi = mach_pi(n);

    finalize(pi, time_start);
}
