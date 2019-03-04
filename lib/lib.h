#include <ctype.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

// Common files for the non-MPI implementations in mach/zeta-0 and 3

int arg_parser(int argc, char** argv, double (*pi_fun)(int), char name[], double& time_start);
void pi_utest(double (*f)(int), char name[]);
void pi_vtest(double (*f)(int), char name[]);
void finalize(double pi, double time_start);
