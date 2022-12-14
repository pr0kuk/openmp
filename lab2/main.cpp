#include "matrix.hpp"
int NUMTHREADS = 1;
void Matrix::red()
{
    stride = 1;
    this->init_f();
    this->init_M();
    for(long long int nn = SIZE, low = 2; nn > 1; nn /= 2, low *= 2, stride *= 2) {
        #pragma omp parallel for num_threads(NUMTHREADS)
        for(long long int i = low - 1; i < SIZE; i += stride * 2) {
            long double alpha = -data[i][i-1] / data[i - stride][i-stride];
            long double gamma = -data[i][i+1] / data[i + stride][i+stride];
            if (i - stride >= 1)
                data[i][i-1] = alpha * data[i - stride][i-stride-1];
            data[i][ i ] = alpha * data[i - stride][i-stride+1] + data[i][i] + gamma * data[i + stride][i+stride-1];
            if (i + stride < SIZE - 1)
                data[i][i+1] = gamma * data[i + stride][i+stride+1];
            f[i] = alpha * f[i - stride] + f[i] + gamma * f[i + stride];
        }
    }
}

void Matrix::obr()
{
    y[SIZE/2] = f[SIZE/2] / data[SIZE/2][SIZE/2];
    for(stride /= 2; stride >= 1; stride /= 2) {
        #pragma omp parallel for num_threads(NUMTHREADS)
        for(long long int i = stride - 1; i < SIZE; i += stride * 2) {
            long double t = f[i];
            if (i - stride > 0)
                t-=data[i][i-1] * y[i - stride];
            if (i + stride < SIZE)
                t-=data[i][i+1] * y[i + stride];
            y[i] = t / data[i][i];
        }
    }

}

void Matrix::calc(long double b) {
    this->b = b;
    for (int i = 0; i < 100; i++)
    {
        this->red();
        this->obr();
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1)
        NUMTHREADS = atoi(argv[1]);
    //std::cout << "NUMTHREADS: " << NUMTHREADS << std::endl;
    Matrix M;
    double start = omp_get_wtime(), end = 0;
    //for (long double i = 0; i <= 1; i += 0.1)
    //    M.calc(i);
    M.calc(0.4);
    M.print_y();
    end = omp_get_wtime();
    //std::cout << "TIME: " << end-start << std::endl;
    return 0;
}