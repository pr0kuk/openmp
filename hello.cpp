#include <iostream>
#include <omp.h>
int main(int argc, char *argv[]) {
    #pragma omp parallel 
    {
        std::cout << omp_get_thread_num() << " Hello world!" << std::endl;
    }
    return 0;
}