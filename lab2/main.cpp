#include "matrix.hpp"

void Matrix::red()
{
    stride = 1;
    this->init_f();
    for(long long int nn = SIZE, low = 2; nn > 1; nn /= 2, low *= 2, stride *= 2) {
        for(long long int i = low - 1; i < SIZE; i += stride * 2) {
            long double alpha = -data[i][i-1] / data[i - stride][i-stride];
            long double gamma = -data[i][i+1] / data[i + stride][i+stride];
            if (i - stride >= 1)
                data[i][i-1] = alpha * data[i - stride][i-stride-1];
            data[i][i] = alpha * data[i - stride][i-stride+1] + data[i][i] + gamma * data[i + stride][i+stride-1];
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

int main()
{
    Matrix M;
    for (int i = 0; i < 10; i++)
    {
        M.init_f();
        M.init_M();
        M.red();
        M.obr();
    }
    M.print_y();
    return 0;
}