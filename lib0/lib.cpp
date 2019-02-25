#include "lib.h"

using namespace std;

void pi_utest(double (*pi_fun)(int), char name[])
{
    printf("Unit test: %s approximation of pi with n = 3\n", name);
    double diff = 3.14 - pi_fun(3);
    abs(diff) < 1 ? puts("Passed") : puts("Failed");
}

void pi_vtest(double (*pi_fun)(int), char name[])
{
    printf("Verification test: %s approximation of pi\n", name);
    ofstream file;
    file.open("vtest.txt");
    puts("writing to vtest.txt ...");
    file << "Verification test: " << name << " approximation of pi\n";
    for (int k = 1; k <= 24; ++k) {
        double diff = fabs(M_PI - pi_fun(pow(2, k)));
        file << "k = " << k << ", diff = " << diff << "\n";
    }

    file.close();
}

int arg_parser(int argc, char** argv, double (*pi_fun)(int), char name[], double& time_start)
{
    if (argc < 2) {
        printf("Requires argument: number of intervals\n");
        return 0;
    }
    time_start = time(NULL);

    int opt;
    while ((opt = getopt(argc, argv, "vu")) != -1) {
        switch (opt) {
        case 'u':
            pi_utest(pi_fun, name);
            return 0;
        case 'v':
            pi_vtest(pi_fun, name);
            return 0;
        }
    }

    return atoi(argv[1]);
}

void finalize(double pi, double time_start)
{
    double error = fabs(M_PI - pi);
    double duration = (double)(time(NULL) - time_start);
    printf("pi=%f, error=%f, duration=%es\n", pi, error, duration);
}
