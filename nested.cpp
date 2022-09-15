#include <iostream>
#include <omp.h>

int main() {
  omp_set_max_active_levels(3);
  omp_set_num_threads(2);
  omp_set_nested(1);
  int num_thread_L0 = omp_get_thread_num(), size_L0 = omp_get_num_threads();
  
  #pragma omp parallel num_threads(2)
  {
    int num_thread_L1 = omp_get_thread_num(), size_L1 = omp_get_num_threads();
    #pragma omp parallel num_threads(2)
    {
      int num_thread_L2 = omp_get_thread_num(), size_L2 = omp_get_num_threads();
      #pragma omp parallel num_threads(2)
      {
        int num_thread_L3 = omp_get_thread_num(), size_L3 = omp_get_num_threads();
        #pragma omp critical 
        {
          std::cout << " num L0 thread: " << num_thread_L0 << " | size L0: " << size_L0 << std::endl << " num L1 thread: " << num_thread_L1 << " | size L1: " << size_L1 << std::endl << " num L2 thread: " << num_thread_L2 << " | size L2: " << size_L2 << std::endl << " num L3 thread: " << num_thread_L3 << " | size L3: " << size_L3 << std::endl << std::endl;
        }
      }
    }
  }
}