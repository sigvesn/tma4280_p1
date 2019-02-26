#include <ctype.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//
// Common files for the MPI implementations in mach/zeta-1, 2, and 4

int arg_parser(int argc, char** argv, int& nprocs, int& rank, double& time_start);
void finalize(int rank, double pi, double time_start);
void gen_limits(int& from, int& to, int rank, int nprocs, int n);
