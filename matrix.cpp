#define THREAD_NUM 12
#define pragmapar for
#define MSIZE 1024
//#define pragmapar for simd
#include <vector>
#include <iostream>
#include <omp.h>
class Matrix {
    public:
    std::vector<std::vector<int>> data;
    void print();
    Matrix();
    Matrix(int X, int Y);
    Matrix(std::vector<std::vector<int>> dat);
};


int m1[MSIZE][MSIZE];
int m2[MSIZE][MSIZE];
int m3[MSIZE][MSIZE];

Matrix::Matrix() {}

Matrix::Matrix(std::vector<std::vector<int>> dat) {
    data = dat;
}

bool operator == (Matrix& m1, Matrix& m2) {
    return m1.data == m2.data;
}

Matrix::Matrix(int x, int y) {
    data = std::vector<std::vector<int>>(x, std::vector<int>(y));
}

std::vector<int> operator + (std::vector<int> v1, std::vector<int> v2) {
    std::vector<int> v3(v1.size());
    if (v1.size() != v2.size() || v1.size() == 0)
        std::cout << "sizes noe equal or == 0" << std::endl;
    else
    #pragma omp parallel pragmapar
        for(int i = 0; i < v1.size(); i++)
            v3[i] = v1[i] + v2[i];
    return v3;
}

std::vector<int> operator - (std::vector<int> v1, std::vector<int> v2) {
    std::vector<int> v3(v1.size());
    if (v1.size() != v2.size() || v1.size() == 0)
        std::cout << "sizes noe equal or == 0" << std::endl;
    else
    #pragma omp parallel pragmapar
        for(int i = 0; i < v1.size(); i++)
            v3[i] = v1[i] - v2[i];
    return v3;
}

std::vector<std::vector<int>> operator + (std::vector<std::vector<int>> v1, std::vector<std::vector<int>> v2) {
    std::vector<std::vector<int>> v3(v1.size(), std::vector<int>(v1[0].size()));
    if (v1.size() != v2.size() || v1.size() == 0)
        std::cout << "sizes noe equal or == 0" << std::endl;
    else
    #pragma omp parallel pragmapar
        for(int i = 0; i < v1.size(); i++)
            v3[i] = v1[i] + v2[i];
    return v3;
}

std::vector<std::vector<int>> operator - (std::vector<std::vector<int>> v1, std::vector<std::vector<int>> v2) {
    std::vector<std::vector<int>> v3(v1.size(), std::vector<int>(v1[0].size()));
    if (v1.size() != v2.size() || v1.size() == 0)
        std::cout << "sizes noe equal or == 0" << std::endl;
    else
    #pragma omp parallel pragmapar
        for(int i = 0; i < v1.size(); i++)
            v3[i] = v1[i] - v2[i];
    return v3;
}

Matrix operator + (Matrix t1, Matrix t2) {
    return Matrix(t1.data + t2.data);
}

Matrix operator - (Matrix t1, Matrix t2) {
    return Matrix(t1.data - t2.data);
}

void Matrix::print() {
    for (auto i:data) {
        for(auto j:i)
            std::cout << j << " ";
        std::cout << std::endl;
    }
}

Matrix operator * (Matrix t1, Matrix t2) {
    std::vector<std::vector<int>> tdata(t1.data.size(), std::vector<int>(t2.data[0].size()));
    int N = t1.data.size();
    #pragma omp parallel for
    for (int x = 0; x < N; x++)
      for (int z = 0; z < N; z++)
        for (int y = 0; y < N; y++)
          tdata[x][z] += t1.data[x][y] * t2.data[y][z];
    return Matrix(tdata);
}


inline void multm (int m1[MSIZE][MSIZE], int m2[MSIZE][MSIZE], int m3[MSIZE][MSIZE]) {
    int N = MSIZE;
    for (int x = 0; x < N; x++) {
      for (int z = 0; z < N; z++) {
        #pragma omp simd reduction(+:m3[x][z])
        for (int y = 0; y < N; y++) {
          m3[x][z] += m1[x][y] * m2[y][z]; 
          }
      }
    }
}

int log2(int x) {
    int full_X = 1;
    while (full_X < x)
        full_X <<= 1;
    return full_X >> 1;
}

void print_sm(int m[MSIZE][MSIZE]) {
    for (int i = 0; i < MSIZE; i++) {
        for (int j = 0; j < MSIZE; j++)
            std::cout << m[i][j] << " ";
        std::cout << std::endl;
    }
}

std::vector<std::vector<Matrix>> separating(Matrix& t) {
    int X = t.data.size(), Y = t.data[0].size();
    int X2 = log2(X), Y2 = log2(Y);
    std::vector<std::vector<Matrix>> ret(2, std::vector<Matrix>(2, Matrix(X2, Y2)));
    #pragma omp parallel pragmapar collapse(2)
    for (int i = 0; i < X2; ++i)
        for (int j = 0; j < Y2; ++j)
            ret[0][0].data[i][j] = t.data[i][j];
    #pragma omp parallel pragmapar collapse(2) 
    for (int i = 0; i < X2; ++i)
        for (int j = Y2; j < Y; ++j)
            ret[0][1].data[i][j - Y2] = t.data[i][j];
    #pragma omp parallel pragmapar collapse(2) 
    for (int i = X2; i < X; ++i)
        for (int j = 0; j < Y2; ++j)
            ret[1][0].data[i - X2][j] = t.data[i][j];
    #pragma omp parallel pragmapar collapse(2) 
    for (int i = X2; i < X; ++i)
        for (int j = Y2; j < Y; ++j)
            ret[1][1].data[i - X2][j - Y2] = t.data[i][j];
    return ret;
}

Matrix unite(std::vector<std::vector<Matrix>>& AB) {
    int X = AB[0][0].data.size() + AB[0][1].data.size();
    int Y = AB[0][0].data.size() + AB[1][0].data.size();
    int X2 = log2(X), Y2 = log2(Y);
    Matrix ret(X, Y);
    #pragma omp parallel pragmapar collapse(2) 
      for (int i = 0; i < X2; i++)
        for (int j = 0; j < Y2; j++)
          ret.data[i][j]= AB[0][0].data[i][j];
    #pragma omp parallel pragmapar collapse(2) 
      for (int i = 0; i < X2; i++)
        for (int j = Y2; j < Y; j++)
          ret.data[i][j] = AB[0][1].data[i][j - Y2];
    #pragma omp parallel pragmapar collapse(2) 
      for (int i = X2; i < X; i++)
        for (int j = 0; j < Y2; j++)
          ret.data[i][j] = AB[1][0].data[i - X2][j];
    #pragma omp parallel pragmapar collapse(2) 
      for (int i = X2; i < X; i++)
        for (int j = Y2; j < Y; j++)
          ret.data[i][j] = AB[1][1].data[i - X2][j - Y2];
    return ret;
}


// Matrix unite(Matrix& A, Matrix& B, Matrix& C, Matrix& D) {
//     int X = A.data.size() + B.data.size();
//     int Y = A.data.size() + C.data.size();
//     int X2 = log2(X), Y2 = log2(Y);
//     Matrix ret(X, Y);
//     #pragma omp parallel pragmapar collapse(2) 
//       for (int i = 0; i < X2; i++)
//         for (int j = 0; j < Y2; j++)
//           ret.data[i][j]= A.data[i][j];
//     #pragma omp parallel pragmapar collapse(2) 
//       for (int i = 0; i < X2; i++)
//         for (int j = Y2; j < Y; j++)
//           ret.data[i][j] = B.data[i][j - Y2];
//     #pragma omp parallel pragmapar collapse(2) 
//       for (int i = X2; i < X; i++)
//         for (int j = 0; j < Y2; j++)
//           ret.data[i][j] = C.data[i - X2][j];
//     #pragma omp parallel pragmapar collapse(2) 
//       for (int i = X2; i < X; i++)
//         for (int j = Y2; j < Y; j++)
//           ret.data[i][j] = D.data[i - X2][j - Y2];
//     return ret;
// }

// Matrix strassen(Matrix& t1, Matrix& t2) {
//     std::vector<std::vector<Matrix>> A = separating(t1), B = separating(t2), AB(2, std::vector<Matrix>(2));
//     auto D = (A[0][0] + A[1][1]) * (B[0][0] + B[1][1]);
//     auto D1 = (A[0][1] - A[1][1]) * (B[1][0] + B[1][1]);
//     auto D2 = (A[1][0] - A[0][0]) * (B[0][0] + B[0][1]);
//     auto H1 = (A[0][0] + A[0][1]) * B[1][1];
//     auto H2 = (A[1][0] + A[1][1]) * B[0][0];
//     auto V1 = A[1][1] * (B[1][0] - B[0][0]);
//     auto V2 = A[0][0] * (B[0][1] - B[1][1]);
//     AB[0][0] = D + D1 - H1 + V1;
//     AB[0][1] = V2 + H1;
//     AB[1][0] = V1 + H2;
//     AB[1][1] = D + D2 + V2 - H2;
//     return unite(AB);
// }


Matrix strassen(Matrix& t1, Matrix& t2) {
    if (t1.data.size() <= 8)
        return t1*t2;
    std::vector<std::vector<Matrix>> A = separating(t1), B = separating(t2), AB(2, std::vector<Matrix>(2));
    Matrix T1(A.size(), (A.size())), T2(A.size(), (A.size())), T3(A.size(), (A.size())), T4(A.size(), (A.size())), T5(A.size(), (A.size())), T6(A.size(), (A.size())), T7(A.size(), (A.size())), T8(A.size(), (A.size())), T9(A.size(), (A.size())), T10(A.size(), (A.size()));//, T11(A.size(), (A.size())), T12(A.size(), (A.size())), T13(A.size(), (A.size())), T14(A.size(), (A.size())), T15(A.size(), (A.size())), T16(A.size(), (A.size())), T17(A.size(), (A.size())), T18(A.size(), (A.size()));
    //Matrix S1(A.size(), (A.size())), S2(A.size(), (A.size())), S3(A.size(), (A.size())), S4(A.size(), (A.size())), S5(A.size(), (A.size())), S6(A.size(), (A.size())), S7(A.size(), (A.size()));
    #pragma omp parallel
    {
    #pragma omp single
    {
    #pragma omp task
    T1 = A[0][0] + A[1][1];
    #pragma omp task
    T2 = B[0][0] + B[1][1];
    #pragma omp task
    T3 = A[1][0] + A[1][1];
    #pragma omp task
    T4 = B[0][1] - B[1][1];
    #pragma omp task
    T5 = B[1][0] - B[0][0];
    #pragma omp task
    T6 = A[0][0] + A[0][1];
    #pragma omp task
    T7 = A[1][0] - A[0][0];
    #pragma omp task
    T8 = B[0][0] + B[0][1];
    #pragma omp task
    T9 = A[0][1] - A[1][1];
    #pragma omp task
    T10 = B[1][0] + B[1][1];
    #pragma omp taskwait
    // std::cout << "end1phase" << std::endl;
    #pragma omp task
    T1 = strassen(T1,T2);
    #pragma omp task
    T3 = strassen(T3,B[0][0]);
    #pragma omp task
    T4 = strassen(A[0][0],T4);
    #pragma omp task
    T5 = strassen(A[1][1],T5);
    #pragma omp task
    T6 = strassen(T6,B[1][1]);
    #pragma omp task
    T7 = strassen(T7,T8);
    #pragma omp task
    T9 = strassen(T9,T10);
    #pragma omp taskwait
    // std::cout << "end2phase" << std::endl;

    #pragma omp task
    T2 = T1 + T5;
    #pragma omp task
    AB[1][0] = T3+T5;
    #pragma omp task
    T7 = T4 + T7;
    #pragma omp task
    T9 = T9-T6;
    #pragma omp task
    AB[0][1] = T4+T6;
    #pragma omp task
    T10 = T1-T3;

    #pragma omp taskwait

    #pragma omp task
    AB[0][0]= T2+T9;
    #pragma omp task
    AB[1][1]=T7+T10;

    #pragma omp taskwait
    }
    }
    return unite(AB);
}

Matrix square_rand_matrix(int siz) {
    Matrix ret(siz, siz);
    for (int i = 0; i < siz; i++)
        for (int j = 0; j < siz; j++)
            ret.data[i][j] = rand() % 6;
    return ret;
}

void sqrmandom(int m[MSIZE][MSIZE]) {
    for (int i = 0; i < MSIZE; i++)
        for (int j = 0; j < MSIZE; j++)
            m[i][j] = rand() % 10;
}

void set_nullm(int m[MSIZE][MSIZE]) {
    for (int i = 0; i < MSIZE; i++)
        for (int j = 0; j < MSIZE; j++)
            m[i][j] = 0;
}

int main(int argc, char* argv[]) {
    int nthr = 1;
    int siz = 16;
    if (argc >= 2)
        nthr = atoi(argv[1]);
    if (argc > 2)
        siz = atoi(argv[2]);
    omp_set_num_threads(nthr);
    // sqrmandom(m1);
    // sqrmandom(m2);
    // set_nullm(m3);
    Matrix t1 = square_rand_matrix(siz), t2 = square_rand_matrix(siz);
    // t1.print();
    // t2.print();
    double start = omp_get_wtime(), end = 0;
    // multm(m1,m2,m3);
    //Matrix a2 = t1 * t2;
    // std::cout << omp_get_wtime() - start << std::endl;
    // std::cout << std::endl;
    // std::cout << "Hey" << std::endl;
    start = omp_get_wtime(), end = 0;
    Matrix a3 = strassen(t1, t2);
    std::cout << omp_get_wtime() - start << std::endl;
    // a2.print();
    std::cout << std::endl;
    std::cout << std::endl;
    //a3.print();
    // if (a2 == a3)
    //     std::cout << "correct" << std::endl;
    // else
    //     std::cout << "incorrect" << std::endl; 
    return 0;
}