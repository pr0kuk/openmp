#define ISIZE 5000
#define JSIZE ISIZE
#define THREAD_NUM 12

#define CHECK(num) do{\
    init_m(b);\
    time = parallel(num, b);\
    fprintf(d, "%d %f\n", num, time);\
    for (int i = 0; i < ISIZE; ++i)\
        for (int j = 0; j < JSIZE; ++j)\
            ASSERT_DOUBLE_EQ(a[i][j], b[i][j]);}while(0)

#include <math.h>
#include <omp.h>
#include <iostream>
#include <stdio.h>
#include "gtest/gtest.h"



double a[ISIZE][JSIZE];
double b[ISIZE][JSIZE];


double parallel(int nthr, double m[ISIZE][JSIZE] )
{
    int i, j;
    double start = omp_get_wtime();
    // #pragma omp parallel for num_threads(nthr) collapse(2) schedule(static, 1)
    for (i=0; i<ISIZE-4; i++){
        #pragma omp parallel for num_threads(nthr)
        for (j = 5; j < JSIZE; j++){
            m[i][j] = sin(0.1*b[i+4][j-5]);
        }
    }
    double time = omp_get_wtime() - start;
    std::cout << nthr << " " << time << std::endl;
    return time;
}

double sequential(double m[ISIZE][JSIZE] )
{
    int i, j;
    double start = omp_get_wtime();
    for (i=0; i<ISIZE-4; i++){
        for (j = 5; j < JSIZE; j++){
            m[i][j] = sin(0.1*m[i+4][j-5]);
        }
    }
    double time = omp_get_wtime() - start;

    std::cout << "1 " << time << std::endl;
    return time;
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


TEST(test, test1) {
    double time = 0;
    init_m(a);
    time = sequential(a);
    FILE* d = fopen("data.txt", "w");
    fprintf(d, "1 %f\n", time);
    for (int i = 2; i <= THREAD_NUM; ++i) {
        CHECK(i);
    }
    // print_m("res_seq.txt", a);
    // print_m("res_par.txt", b);
    
}

int main(int argc, char * argv[]) {
  ::testing::InitGoogleTest (&argc , argv);
  return RUN_ALL_TESTS ();
}