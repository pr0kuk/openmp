#define ISIZE 5000
#define JSIZE ISIZE

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
    int tsize = (JSIZE - 3) / commsize;
    int start_elem = my_rank * tsize;
    int end_elem = (my_rank == commsize - 1) ? JSIZE - 3 : start_elem + tsize;
    int size = end_elem - start_elem;
    if (my_rank == 0)
        for (int i = 0; i < commsize; i++)
            sizes[i] = (i == commsize - 1) ? JSIZE - 3 - i * tsize : tsize;
    double start = MPI_Wtime();
    for (int i = 2; i < ISIZE; ++i) {
        for (int j = start_elem; j < end_elem; ++j)
            m[i][j] = sin(5*m[i-2][j+3]);

        if (my_rank != 0)
            MPI_Send(&(m[i][start_elem]), 3, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD);
        if (my_rank != commsize - 1)
            MPI_Recv(&(m[i][end_elem]), 3, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (my_rank != 0)
        for (int i = 2; i < ISIZE; i++)
            MPI_Send(&(m[i][start_elem]), size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    if (my_rank == 0)
        for (int j = 1; j < commsize; j++)
            for (int i = 2; i < ISIZE; i++)
                MPI_Recv(&(m[i][j * tsize]), sizes[j], MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


    double time = MPI_Wtime()- start;
    if (my_rank == 0)
        std::cout << commsize << " " << time << std::endl;
}

void sequential(double m[ISIZE][JSIZE] )
{
    // double start = MPI_Wtime();
    for (int i = 2; i < ISIZE; i++)
        for (int j = 0; j < JSIZE - 3; j++)
            m[i][j] = sin(5 * m[i - 2][j + 3]);
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