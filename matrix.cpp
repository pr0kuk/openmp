#include <vector>
#include <iostream>
class Matrix {
    public:
    std::vector<std::vector<int>> data;
    void print();
    Matrix();
    Matrix(int X, int Y);
    Matrix(std::vector<std::vector<int>> dat);
};

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
        for(int i = 0; i < v1.size(); i++)
            v3[i] = v1[i] + v2[i];
    return v3;
}

std::vector<int> operator - (std::vector<int> v1, std::vector<int> v2) {
    std::vector<int> v3(v1.size());
    if (v1.size() != v2.size() || v1.size() == 0)
        std::cout << "sizes noe equal or == 0" << std::endl;
    else
        for(int i = 0; i < v1.size(); i++)
            v3[i] = v1[i] - v2[i];
    return v3;
}

std::vector<std::vector<int>> operator + (std::vector<std::vector<int>> v1, std::vector<std::vector<int>> v2) {
    std::vector<std::vector<int>> v3(v1.size(), std::vector<int>(v1[0].size()));
    if (v1.size() != v2.size() || v1.size() == 0)
        std::cout << "sizes noe equal or == 0" << std::endl;
    else
        for(int i = 0; i < v1.size(); i++)
            v3[i] = v1[i] + v2[i];
    return v3;
}

std::vector<std::vector<int>> operator - (std::vector<std::vector<int>> v1, std::vector<std::vector<int>> v2) {
    std::vector<std::vector<int>> v3(v1.size(), std::vector<int>(v1[0].size()));
    if (v1.size() != v2.size() || v1.size() == 0)
        std::cout << "sizes noe equal or == 0" << std::endl;
    else
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
    for (int x = 0; x < t1.data.size(); ++x)
      for (int z = 0, sum = 0; z < t2.data[0].size(); tdata[x][z++] = sum, sum = 0) {
        for (int y = 0; y < t1.data[0].size(); ++y)
          sum += t1.data[x][y] * t2.data[y][z];
        //std::cout << sum << std::endl;
      }
    return Matrix(tdata);
}

int log2(int x) {
    int full_X = 1;
    while (full_X < x)
        full_X <<= 1;
    return full_X >> 1;
}

std::vector<std::vector<Matrix>> separating(Matrix& t) {
    int X = t.data.size(), Y = t.data[0].size();
    int X2 = log2(X), Y2 = log2(Y);
    std::vector<std::vector<Matrix>> ret(2, std::vector<Matrix>(2, Matrix(X2, Y2)));
      for (int i = 0; i < X2; ++i)
        for (int j = 0; j < Y2; ++j)
          ret[0][0].data[i][j] = t.data[i][j];
      for (int i = 0; i < X2; ++i)
        for (int j = Y2; j < Y; ++j)
          ret[0][1].data[i][j - Y2] = t.data[i][j];
      for (int i = X2; i < X; ++i)
        for (int j = 0; j < Y2; ++j)
          ret[1][0].data[i - X2][j] = t.data[i][j];
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
      for (int i = 0; i < X2; i++)
        for (int j = 0; j < Y2; j++)
          ret.data[i][j]= AB[0][0].data[i][j];
      for (int i = 0; i < X2; i++)
        for (int j = Y2; j < Y; j++)
          ret.data[i][j] = AB[0][1].data[i][j - Y2];
      for (int i = X2; i < X; i++)
        for (int j = 0; j < Y2; j++)
          ret.data[i][j] = AB[1][0].data[i - X2][j];
      for (int i = X2; i < X; i++)
        for (int j = Y2; j < Y; j++)
          ret.data[i][j] = AB[1][1].data[i - X2][j - Y2];
    return ret;
}

Matrix strassen(Matrix& t1, Matrix& t2) {
    std::vector<std::vector<Matrix>> A = separating(t1), B = separating(t2), AB(2, std::vector<Matrix>(2));
    auto D = (A[0][0] + A[1][1]) * (B[0][0] + B[1][1]);
    auto D1 = (A[0][1] - A[1][1]) * (B[1][0] + B[1][1]);
    auto D2 = (A[1][0] - A[0][0]) * (B[0][0] + B[0][1]);
    auto H1 = (A[0][0] + A[0][1]) * B[1][1];
    auto H2 = (A[1][0] + A[1][1]) * B[0][0];
    auto V1 = A[1][1] * (B[1][0] - B[0][0]);
    auto V2 = A[0][0] * (B[0][1] - B[1][1]);
    AB[0][0] = D + D1 - H1 + V1;
    AB[0][1] = V2 + H1;
    AB[1][0] = V1 + H2;
    AB[1][1] = D + D2 + V2 - H2;
    return unite(AB);
}

int main() {
    Matrix t1({{1, 1}, {1,1}, {10, 10}});
    Matrix t2({{5,7,2},{3,3,4}});
    Matrix a2 = t1 * t2;
    a2.print();
    std::cout << std::endl;
    Matrix a3 = strassen(t1, t2);
    a3.print();
}