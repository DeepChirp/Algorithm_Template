/*
 * name: 快速傅里叶变换
 * description:
 *
 * 设$f(x) = a_0x^0 + a_1x^1 + \ldots + a_{n - 1}x^{n -
 * 1}$，则FFT可求出$[f(\omega_n^0), f(\omega_n^1), \ldots, f(\omega_n^{n - 1})]$
 *
 * 时间复杂度：$O(N\log{N})$
 *
 * Verdict：C6 C
 */

#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdio>
#include <vector>

#define EPS 1e-6

typedef double number;
typedef long long i64;

double const PI = std::acos(-1);

using Complex = std::complex<number>;

// 同样需要保证 len 是 2 的幂
// 记 rev[i] 为 i 翻转后的值
void change(std::vector<Complex> &y) {
    std::vector<size_t> rev;
    size_t len = y.size();
    rev.resize(len, 0);

    for (size_t i = 0; i < len; ++i) {
        rev[i] = rev[i >> 1] >> 1;
        if (i & 1) { // 如果最后一位是 1，则翻转成 len/2
            rev[i] |= len >> 1;
        }
    }
    for (size_t i = 0; i < len; ++i) {
        if (i < rev[i]) { // 保证每对数只翻转一次
            std::swap(y[i], y[rev[i]]);
        }
    }
    return;
}

/*
 * 做 FFT
 * len 必须是 2^k 形式
 * reverse == false 时是 DFT，reverse == true 时是 IDFT
 */
void fft(std::vector<Complex> &y, bool reverse) {
    size_t len = y.size();

    // 位逆序置换

    change(y);

    int on = 1;
    if (reverse) {
        on = -1;
    }

    // 模拟合并过程，一开始，从长度为一合并到长度为二，一直合并到长度为
    // len。
    for (size_t h = 2; h <= len; h <<= 1) {
        // wn：当前单位复根的间隔：w^1_h
        Complex wn(cos(2 * PI / (double)h), sin(on * 2 * PI / (double)h));
        // 合并，共 len / h 次。
        for (size_t j = 0; j < len; j += h) {
            // 计算当前单位复根，一开始是 1 = w^0_n，之后是以 wn
            // 为间隔递增： w^1_n
            // ...
            Complex w(1, 0);
            for (size_t k = j; k < j + h / 2; k++) {
                // 左侧部分和右侧是子问题的解
                Complex u = y[k];
                Complex t = w * y[k + h / 2];
                // 这就是把两部分分治的结果加起来
                y[k] = u + t;
                y[k + h / 2] = u - t;
                // 后半个 「step」 中的ω一定和 「前半个」 中的成相反数
                // 「红圈」上的点转一整圈「转回来」，转半圈正好转成相反数
                // 一个数相反数的平方与这个数自身的平方相等
                w = w * wn;
            }
        }
    }
    // 如果是 IDFT，它的逆矩阵的每一个元素不只是原元素取倒数，还要除以长度
    // len。
    if (on == -1) {
        for (size_t i = 0; i < len; i++) {
            y[i].real(y[i].real() / (double)len);
            y[i].imag(y[i].imag() / (double)len);
        }
    }
}