/*
 * name: 最大公约数（拓展欧几里得算法）、线性同余方程、中国剩余定理
 * description:
 *
 * Verdict：B4025：https://www.luogu.com.cn/record/190564296
 * Verdict：P1082：https://www.luogu.com.cn/record/190565417
 * Verdict：P1495：https://www.luogu.com.cn/record/190793944
 * 注：P1495的一个WA由i64溢出导致，相信出题不会这么阴间
 */

#include <cstdlib>
#include <numeric>
#include <tuple>
#include <utility>
#include <vector>

typedef long long number;

number gcd(number a, number b) {
    a = std::abs(a);
    b = std::abs(b);
    if (a < b) {
        std::swap(a, b);
    }

    while (b > 0) {
        number temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

// d = gcd(a, b) = ax + by
struct EEResult {
    number d;
    number x;
    number y;
};

EEResult exgcd(number a, number b) {
    a = std::abs(a);
    b = std::abs(b);

    number x = 1, y = 0;

    number x1 = 0, y1 = 1, a1 = a, b1 = b;
    while (b1 > 0) {
        number q = a1 / b1;
        std::tie(x, x1) = std::make_tuple(x1, x - q * x1);
        std::tie(y, y1) = std::make_tuple(y1, y - q * y1);
        std::tie(a1, b1) = std::make_tuple(b1, a1 - q * b1);
    }
    return EEResult{a1, x, y};
}

// Solve ax = b (mod n) for x = x_0
// x_i = x_0 + i * (n / d), d = gcd(a, n)
// solvable if and only if: d | b

struct MLEResult {
    bool solvable;
    number x0;
};

MLEResult modular_linear_equation_solver(number a, number b, number n) {
    EEResult r = exgcd(a, n);

    number d = r.d;

    if (b % d == 0) {
        return MLEResult{true, ((r.x * b / d) + n) % n};
    } else {
        return MLEResult{false, 0};
    }
}

// n = n_1n_2...n_k
// a = (a_1c_1 + a_2c_2 + ... + a_nc_n) mod n
struct CRTResult {
    number a;
    number n;
};

// n_1, n_2, ..., n_k are pairwise relative prime
// 求同余方程组：x = a_i (mod n_i)的解：x = a (mod n)
CRTResult crt(std::vector<number> const &a_list,
              std::vector<number> const &n_list) {
    number n = std::accumulate(std::begin(n_list), std::end(n_list), 1LL,
                               std::multiplies<number>{});

    number result = 0;
    for (size_t i = 0; i < n_list.size(); i++) {
        number n_i = n_list[i];
        number m_i = n / n_i;

        MLEResult r = modular_linear_equation_solver(m_i, 1, n_i);

        number c_i = (r.x0 % n * m_i % n) % n;

        result = (result + (a_list[i] * c_i) % n) % n;
    }

    return CRTResult{result, n};
}
