#define ISIZE 5000
#define JSIZE ISIZE
#define THREAD_NUM 12

#define CHECK(num) do{\
    init_m(b);\
    time = parallel(num, b);\
    fprintf(d, "%d %f\n", num, time);\
    for (int i = 0; i < ISIZE; ++i)\
        for (int j = 0; j < JSIZE; ++j)\
            ASSERT_DOUBLE_EQ(a[i][j], b[i][j]);} while(0)

#include <math.h>
#include <omp.h>
#include <iostream>
#include <stdio.h>
#include "gtest/gtest.h"

double a[ISIZE][JSIZE];
double b[ISIZE][JSIZE];


double parallel(int nthr, double m[ISIZE][JSIZE])
{
    double start = omp_get_wtime();
    #pragma omp parallel for collapse(2) num_threads(nthr)
    for (int i = 0; i < ISIZE-4; i++) {
        for (int j = 0; j < JSIZE; j++)
            m[i][j] = sin(2 * b[i][j]);
    }
    double time = omp_get_wtime() - start;
    std::cout << nthr << " " << time << std::endl;
    return time;
}

double sequential(double m[ISIZE][JSIZE] )
{
    double start = omp_get_wtime();
    for (int i = 0; i < ISIZE - 4; i++) {
        for (int j = 0; j < JSIZE; j++)
            m[i][j] = sin(2 * m[i][j]);
    }
    double time = omp_get_wtime() - start;
    std::cout << "1 " << time << std::endl;
    return time;
}

void print_m(const char* const s, double m[ISIZE][JSIZE])
{
    FILE* ff = fopen(s, "w");
    for(int i = 0; i < ISIZE; i++){
        for (int j=  0; j < JSIZE; j++)
            fprintf(ff, "%f ", m[i][j]);
        fprintf(ff, "\n");
    }
    fclose(ff);
}

void init_m(double m[ISIZE][JSIZE] ) {
    for (int i = 0; i < ISIZE; i++)
        for (int j=0; j < JSIZE; j++)
            m[i][j] = 10*i +j;
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