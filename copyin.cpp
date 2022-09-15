#include <iostream>
#include <omp.h>

int n;
#pragma omp threadprivate(n)


int main(){
    omp_set_num_threads(4);
    n = 908;
    #pragma omp parallel
    {
        n = omp_get_thread_num();
    }
    std::cout << "Default\n";
    #pragma omp parallel
    {
        #pragma omp critical
        {
            std::cout << "n: " << n << " thread: " << omp_get_thread_num() << "\n";
        }
    }

    n = 908;
    std::cout << "\nCopyin\n";
    #pragma omp parallel copyin(n)
    {
        #pragma omp critical
        {
            std::cout << "n: " << n << " thread: " << omp_get_thread_num() << "\n";
        }
    } 
}