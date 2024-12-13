/*
 * name: 大整数乘法
 * description:
 *
 * 使用FFT实现的大整数乘法。
 * 包含进位、去除最高位的0（见main函数）。
 *
 * 时间复杂度：$O(N\log{N})$
 *
 * Verdict：C6 D
 * Verdict：P1919： https://www.luogu.com.cn/record/191419431
 */

#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstring>
#include <vector>

#define BUFFER_LEN 1000005
#define EPS 1e-6

typedef double number;
typedef long long i64;

double const PI = std::acos(-1);

char buffer[BUFFER_LEN] = {0};

using Complex = std::complex<number>; // STL complex
void change(Complex y[], int len);
void fft(Complex y[], int len, int on);
std::vector<number> get_big_int();

class Multiplier {
  private:
    std::vector<Complex> a_coff_list;
    std::vector<Complex> b_coff_list;
    size_t a_len;
    size_t b_len;
    size_t input_len;

  public:
    explicit Multiplier(std::vector<number> const &a_list,
                        std::vector<number> const &b_list) {
        input_len = std::max(a_list.size(), b_list.size());
        size_t len = next_power_of_two(2 * input_len);

        a_len = a_list.size();
        b_len = b_list.size();

        a_coff_list.resize(len);
        b_coff_list.resize(len);

        for (size_t i = 0; i < a_list.size(); i++) {
            a_coff_list[i].real(a_list[i]);
        }

        for (size_t i = 0; i < b_list.size(); i++) {
            b_coff_list[i].real(b_list[i]);
        }
    }

    // 对于一个Multiplier对象，该函数仅允许调用一次！
    std::vector<number> multiply() {
        fft(a_coff_list, false);
        fft(b_coff_list, false);

        std::vector<Complex> c_coff_list;
        c_coff_list.resize(a_coff_list.size());

        std::transform(std::begin(a_coff_list), std::end(a_coff_list),
                       std::begin(b_coff_list), std::begin(c_coff_list),
                       [](Complex a, Complex b) { return a * b; });

        fft(c_coff_list, true);

        std::vector<number> result;
        result.resize(a_len + b_len - 1);

        std::transform(std::begin(c_coff_list),
                       std::begin(c_coff_list) + (long)(a_len + b_len - 1),
                       std::begin(result), [](Complex a) { return a.real(); });

        return result;
    }

  private:
    static size_t next_power_of_two(size_t input) {
        if (input == 0) {
            return 1;
        }

        input--;

        input |= (input >> 1);
        input |= (input >> 2);
        input |= (input >> 4);
        input |= (input >> 8);
        input |= (input >> 16);
        input |= (input >> 32);

        return (input + 1);
    }

    // 同样需要保证 len 是 2 的幂
    // 记 rev[i] 为 i 翻转后的值
    static void change(std::vector<Complex> &y) {
        std::vector<size_t> rev;
        size_t len = y.size();
        rev.resize(len, 0);

        for (size_t i = 0; i < len; ++i) {
            rev[i] = rev[i >> 1] >> 1;
            if (i % 2 != 0) { // 如果最后一位是 1，则翻转成 len/2
                rev[i] |= len >> 1;
            }
        }
        for (size_t i = 0; i < len; ++i) {
            if (i < rev[i]) { // 保证每对数只翻转一次
                std::swap(y[i], y[rev[i]]);
            }
        }
    }

    /*
     * 做 FFT
     * len 必须是 2^k 形式
     * reverse == false 时是 DFT，reverse == true 时是 IDFT
     */
    static void fft(std::vector<Complex> &y, bool reverse) {
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
            }
        }
    }
};

int main(void) {
    std::vector<number> a_digit_list = get_big_int();
    std::vector<number> b_digit_list = get_big_int();

    Multiplier mul{a_digit_list, b_digit_list};

    std::vector<number> temp = mul.multiply();

    std::vector<int> result;
    result.resize(2 * temp.size(), 0);

    int carry = 0;

    // 进位
    for (size_t i = 0; i < temp.size(); i++) {
        int current_digit = (int)std::lround(temp[i]);

        current_digit += carry;

        result[i] = current_digit % 10;
        carry = current_digit / 10;
    }

    size_t result_len = 0;

    // 进位
    for (size_t i = temp.size(); i < result.size(); i++) {
        int current_digit = carry;

        result[i] = current_digit % 10;
        carry = current_digit / 10;

        if (carry == 0 && result[i] == 0) {
            result_len = i;
            break;
        }
    }

    for (size_t i = 0; i < result_len; i++) {
        std::putchar(result[result_len - i - 1] + '0');
    }

    std::putchar('\n');

    return 0;
}

std::vector<number> get_big_int() {
    std::vector<number> result;
    std::scanf("%s", buffer);

    size_t num_len = std::strlen(buffer);

    result.resize(num_len, 0);

    for (size_t i = 0; i < num_len; i++) {
        result[num_len - i - 1] = (number)(buffer[i] - '0');
    }

    return result;
}