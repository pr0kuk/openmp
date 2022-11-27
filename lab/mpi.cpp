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


double parallel(double m[ISIZE][JSIZE])
{
    int sizes[commsize];
    int cur2Size = (JSIZE - 3) / (commsize);
    auto start_elem = (my_rank) * cur2Size;
    auto end_elem = start_elem + cur2Size;
    if (my_rank == commsize - 1)
        end_elem = JSIZE-3;
    auto size = end_elem - start_elem;
    // std::cout << "size is " << size << " " << my_rank << std::endl;
    if (my_rank == 0) {
        for (int i = 0; i < commsize; i++) {
            sizes[i] = (i == commsize - 1) ? JSIZE -3 - i * cur2Size : cur2Size;
        }
    }
    double start = MPI_Wtime();
    for (int i = 2; i < ISIZE; ++i) {
        for (int j = start_elem; j < end_elem; ++j) {
            m[i][j] = sin(5*m[i-2][j+3]);
        }
        if (my_rank != 0) {
            MPI_Send(&(m[i][start_elem]), size, MPI_DOUBLE, /*dest*/ 0, 0, MPI_COMM_WORLD);
        }
        if(my_rank == 0) {
            for (int j = 1; j < commsize; ++j) {
                MPI_Recv(&(m[i][j * cur2Size]), sizes[j], MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }

        if (my_rank != 0)
            MPI_Send(&(m[i][start_elem]), 3, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD);
        if (my_rank != commsize - 1)
            MPI_Recv(&(m[i][end_elem]), 3, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }


    double time = MPI_Wtime()- start;
    if (my_rank == 0)
        std::cout << commsize << " " << time << std::endl;
    return 0;
}

double sequential(double m[ISIZE][JSIZE] )
{
    int i, j;
    double start = MPI_Wtime();
    for (i=2; i<ISIZE; i++){
        for (j = 0; j < JSIZE-3; j++){
            m[i][j] = sin(5*m[i-2][j+3]);
        }
    }
    double time = MPI_Wtime() - start;
    std::cout << "1 " << time << std::endl;
    return 0;
}

int print_m(const char* const s, double m[ISIZE][JSIZE])
{
    FILE* ff = fopen(s, "w");
    for(int i=0; i < ISIZE; i++){
        for (int j=0; j < JSIZE; j++){
            fprintf(ff,"%f ",m[i][j]);
        }
    fprintf(ff,"\n");
    }
    fclose(ff);
    return 0;
}

int init_m(double m[ISIZE][JSIZE] ) {
    int i = 0, j = 0;
    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            m[i][j] = 10*i +j;
        }
    }
    return 0;
}




int main(int argc, char * argv[]) {
    targc = argc;
    targv = argv;
    MPI_Init(&targc, &targv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
    init_m(b);
    parallel(b);
    // if (my_rank == 0){
    //     init_m(a);
    //     sequential(a);
    //     // print_m("testa.txt", a);
    //     // print_m("testb.txt", b);
    //     for (int i = 0; i < ISIZE; ++i) {
    //         for (int j = 0; j < JSIZE; ++j) {
    //             // std::cout << i << " " << j << " " << a[i][j] << " " << b[i][j] << std::endl;
    //             assert(a[i][j] == b[i][j]);}
    //     }
    // }
    MPI_Finalize();
}