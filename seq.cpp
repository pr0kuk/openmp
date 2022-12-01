#include <iostream>
#include <omp.h>
#include <string>

int main(int argc, char *argv[]) {
  int s = 0;
  int NumThreads = omp_get_max_threads();

#pragma omp parallel for ordered
  for (int i = 0; i < NumThreads; ++i) {
    #pragma omp critical
    std::cout << "thr: " << omp_get_thread_num() << " i: " << i << " s: " << s++ << std::endl;
  }
}