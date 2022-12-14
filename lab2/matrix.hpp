#pragma once
#include <vector>
#include <random>
#include <iostream>
#include <omp.h>
#define SIZE 4095

class Matrix {
    public:
    std::vector<std::vector<long double>> data = std::vector<std::vector<long double>>(SIZE, std::vector<long double>(SIZE,0));;
    std::vector<long double> f = std::vector<long double>(SIZE,0);
    std::vector<long double> y = std::vector<long double>(SIZE,0);
    const long double hh = (long double)1/SIZE/SIZE;
    long long int stride = 1;
    void red();
    void obr();
    void print();
    void print_y();
    void init_M();
    void init_f();
    void init_y();
    Matrix() {init_M(); init_y(); init_f();}
};

void Matrix::init_M()
{
    data[0][0] = 1;
    data[SIZE-1][SIZE-1] = 1;
    for (long long int i = 1; i < SIZE-1; i++)
    {
        for (long long int j = 0; j < SIZE; j++)
        {
            if (i == j)
                data[i][j] = (long double)(-2) / hh;
            if (i == j - 1 || j == i - 1)
                data[i][j] = (long double)(1) / hh;
        }
    }
}

void Matrix::init_y ()
{
    // std::random_device rd;  //Will be used to obtain a seed for the random number engine
    // std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    // std::uniform_int_distribution<> distrib(1, v.size());
    // for (auto i = y.begin(); i<y.end(); i++)
    //     *i = distrib(gen);
    // y[0] = 1;
    // y[v.size() - 1] = b;
    for (int i = 0; i < SIZE; i++)
        y[i] = 1;
}

void Matrix::init_f ()
{
    f[0] = 1;
    f[SIZE-1] = 0;
    for (long long int i = 1; i < SIZE-1; i++) {
        f[i] = (exp(y[i])+(exp(y[i+1])- 2*exp(y[i]) + exp(y[i-1]))/12);
    }

}

void Matrix::print() {
    for (auto i:data) {
        for(auto j:i)
            std::cout << j << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (auto i:f)
        std::cout << i << " ";
    std::cout << std::endl;

    for (auto i:y)
        std::cout << i << " ";
    std::cout << std::endl;
}

void Matrix::print_y() {
    for (auto i:y)
        std::cout << i << " ";
    std::cout << std::endl;
}