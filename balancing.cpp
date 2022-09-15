#define NTHREADS 4
#define NITER 65
#include <omp.h>
#include <iostream>

void f(int i) {
    #pragma omp critical 
    {
    std::cout << i << " - " << omp_get_thread_num() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    std::cout << "default:" << std::endl;
    #pragma omp parallel for num_threads(NTHREADS)
        for (int i = 0; i < NITER; ++i)
            f(i);
    std::cout << std::endl;
    std::cout << "static 1:" << std::endl;
    #pragma omp parallel for schedule(static, 1) num_threads(NTHREADS)
        for (int i = 0; i < NITER; ++i)
            f(i);
    std::cout << std::endl;
    std::cout << "static 4:" << std::endl;
    #pragma omp parallel for schedule(static, 4) num_threads(NTHREADS)
        for (int i = 0; i < NITER; ++i)
            f(i);
    std::cout << std::endl;
    std::cout << "dynamic 1:" << std::endl;
    #pragma omp parallel for schedule(dynamic, 1) num_threads(NTHREADS)
        for (int i = 0; i < NITER; ++i)
            f(i);
    std::cout << std::endl;
    std::cout << "dynamic 4:" << std::endl;
    #pragma omp parallel for schedule(dynamic, 4) num_threads(NTHREADS)
        for (int i = 0; i < NITER; ++i)
            f(i);
    std::cout << std::endl;
    std::cout << "guided 1:" << std::endl;
    #pragma omp parallel for schedule(guided, 1) num_threads(NTHREADS)
        for (int i = 0; i < NITER; ++i)
            f(i);
    std::cout << std::endl;
    std::cout << "guided 4:" << std::endl;
    #pragma omp parallel for schedule(guided, 4) num_threads(NTHREADS)
        for (int i = 0; i < NITER; ++i)
            f(i);
}