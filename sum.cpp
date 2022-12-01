#include <cmath>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "argc < 2";
        return 1;
    }
    int n = std::stoi(argv[1]);
    double sum = 1.0;
    #pragma omp parallel for reduction(+ : sum) num_threads(n)
    for (int i = 2; i <= n; ++i) {
        sum += 1.0 / i;
    }
    std::cout << sum << std::endl;
    return 0;
}