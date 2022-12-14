import numpy as np
import math
Q = 3
N = 2**Q-1
h = 1/N
h2 = h * h
err = 10
err_p = 1e-3


def geterr(yn, y):
    return sum((yn-y)**2)**0.5
def getf(y):
    fn = np.ones(N)
    fn[0] = 1
    fn[N-1] = 1
    for i in range(1,N-1):
        fn[i] = (math.exp(y[i])+(math.exp(y[i+1])- 2*math.exp(y[i]) + math.exp(y[i-1]))/12)
    return fn

def init_M():
    M = np.zeros((N, N))
    M[0][0] = 1
    M[N-1][N-1] = 1
    for i in range(1,N-1):
        M[i][i] = -2 /h2
        if (i > 0):
            M[i][i-1] = 1 /h2
        if (i < N - 1):
            M[i][i+1] = 1 /h2
    return M

def prep():
    return np.ones(N), np.ones(N), getf(np.ones(N)), init_M()

y,yn,f,M = prep()

for j in range(10):
    yn = np.linalg.solve(M, f)
    f = getf(yn)
    y = yn.copy()

file = open('sam.txt', 'w')
s = ''
for i in y:
    s += str(i) + ' '
file.write(s)
file.write('\n')