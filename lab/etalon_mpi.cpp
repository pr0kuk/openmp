#define ISIZE 5000
#define JSIZE 5000

#include <math.h>
#include "mpi.h"
#include <iostream>
#include <stdio.h>
#include "gtest/gtest.h"
#include <cassert>
int targc;
char** targv;
int commsize = 0, my_rank = 0;
double a[ISIZE][JSIZE];
double b[ISIZE][JSIZE];


void parallel(double m[ISIZE][JSIZE])
{
    int sizes[commsize];
    int tsize = (ISIZE) / commsize;
    int start_str = my_rank * tsize;
    int end_str= (my_rank == commsize - 1) ? ISIZE : start_str + tsize;;
    int size = end_str - start_str;
    if (my_rank == 0)
        for (int i = 0; i < commsize; i++)
            sizes[i] = (i == commsize - 1) ? ISIZE - i * tsize : tsize;
    double start = MPI_Wtime();
    for (int i = start_str; i < end_str; ++i) {
        for (int j = 0; j < JSIZE; ++j) {
            m[i][j] = sin(2*m[i][j]);
        }
    }

    if (my_rank != 0)
        MPI_Send(&(m[start_str]), size*JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    if (my_rank == 0)
        for (int j = 1; j < commsize; j++)
            MPI_Recv(&(m[j*tsize]), sizes[j]*JSIZE, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


    double time = MPI_Wtime()- start;
    if (my_rank == 0)
        std::cout << commsize << " " << time << std::endl;
}

void sequential(double m[ISIZE][JSIZE] )
{
    // double start = MPI_Wtime();
    for (int i = 0; i < ISIZE; i++)
        for (int j = 0; j < JSIZE; j++)
            m[i][j] = sin(2 * m[i][j]);
    // double time = MPI_Wtime() - start;
    // std::cout << "1 " << time << std::endl;
}

void print_m(const char* const s, double m[ISIZE][JSIZE])
{
    FILE* ff = fopen(s, "w");
    for(int i = 0; i < ISIZE; i++) {
        for (int j=0; j < JSIZE; j++)
            fprintf(ff,"%f ",m[i][j]);
        fprintf(ff,"\n");
    }
    fclose(ff);
}

void init_m(double m[ISIZE][JSIZE] ) {
    for (int i = 0; i < ISIZE; i++)
        for (int j = 0; j < JSIZE; j++)
            m[i][j] = 10 * i + j;
}

int main(int argc, char * argv[]) {
    targc = argc;
    targv = argv;
    MPI_Init(&targc, &targv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
    init_m(b);
    parallel(b);
    if (my_rank == 0){
        init_m(a);
        sequential(a);
        // print_m("testa.txt", a);
        // print_m("testb.txt", b);
        for (int i = 0; i < ISIZE; ++i) {
            for (int j = 0; j < JSIZE; ++j) {
                assert(a[i][j] == b[i][j]);}
        }
    }
    MPI_Finalize();
}