/*
 * name: FFT Algorithm
 * description:
 *
 * FFT is a function that computes the Discrete Fourier Transform (DFT) of a
 * sequence, or its inverse (IDFT).
 *
 */
#include <algorithm>
#include <complex>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read(void) {
    int x, f = 1;
    char ch;
    while (!isdigit(ch = getchar()))
        (ch == '-') && (f = -1);
    for (x = ch ^ 48; isdigit(ch = getchar());
         x = (x << 1) + (x << 3) + (ch ^ 48))
        ;
    return x * f;
}

void write(int x) {
    if (x < 0)
        putchar('-'), x = -x;
    if (x > 9)
        write(x / 10);
    putchar(x % 10 + '0');
    return;
}

const int maxn = 1 << 22;
int numberIndex[maxn];
int n, m, upBound = 1;
std ::complex<double> a[maxn], b[maxn];
std ::complex<double> w[maxn];

std ::complex<double> A[maxn], A1[maxn], A2[maxn];
void FFT(std ::complex<double> *a, int n, int type) {
    for (int i = 0; i < n; ++i)
        A[numberIndex[i]] = a[i];
    for (int mid = 1; mid <= n >> 1; mid <<= 1) {
        int lenOfInterval = mid << 1;
        double lenOfIntervalDouble = lenOfInterval * 1.0,
               theta = 2.0 * acos(-1.0) / lenOfIntervalDouble;
        std ::complex<double> Wn = {cos(theta), (type ? -1 : 1) * sin(theta)};
        for (int l = 0; l < n; l += lenOfInterval) {
            std ::complex<double> w = {1, 0};
            for (int p = l; p < l + mid; ++p) {
                std ::complex<double> temp1 = A[p], temp2 = w * A[mid + p];
                A[p] = temp1 + temp2;
                A[p + mid] = temp1 - temp2;
                w *= Wn;
            }
        }
    }
    for (int i = 0; i < n; ++i)
        a[i] = A[i];
}

std ::complex<double> Ares[maxn], Bres[maxn];

int main() {
    n = read(), m = read();

    while (upBound <= n + m)
        upBound <<= 1;

    for (int i = 0; i < upBound; ++i)
        numberIndex[i] =
            (numberIndex[i >> 1] >> 1) | ((i & 1) ? (upBound >> 1) : 0);

    for (int i = 0; i <= n; ++i)
        a[i] = read();
    for (int i = 0; i <= m; ++i)
        b[i] = read();

    FFT(a, upBound, 0);
    FFT(b, upBound, 0);

    for (int i = 0; i <= upBound; ++i)
        a[i] = a[i] * b[i];

    FFT(a, upBound, 1);

    for (int i = 0; i <= n + m; ++i) {
        double ans = a[i].real() / upBound + 0.5;
        int x = ans;
        printf("%d ", x);
    }
}