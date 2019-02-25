#include <lib.h>

double zeta(int n)
{
    double s = 0.0;

    for (int i = 1; i < n; ++i)
        s += 1 / pow(i, 2);

    return s;
}

double zeta_pi(int n)
{
    return sqrt(zeta(n) * 6);
}

int main(int argc, char** argv)
{

    char name[] = "Serial Zeta";
    double time_start;
    const int n = arg_parser(argc, argv, zeta_pi, name, time_start);
    if (n <= 0)
        return 0;

    printf("Serial Zeta approx of pi with n = %d\n", n);

    double pi = zeta_pi(n);

    finalize(pi, time_start);
}
