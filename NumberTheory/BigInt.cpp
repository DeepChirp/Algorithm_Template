/*
 * name: 高精度
 * description:
 * 注意：只处理了正数
 * 注意：此处的高精度乘法采用朴素方法
 * 注意：保证所有操作后（+，-，*），得到的len反映了实际的数位长度（即，考虑了进位，并删去了前导零）
 *
 * Verdict：P1601：https://www.luogu.com.cn/record/190643889
 * Verdict：P1303：https://www.luogu.com.cn/record/190648675
 */

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

struct BigInt {
    std::vector<int> digits;
    size_t len;

    explicit BigInt(size_t capacity) {
        digits.resize(capacity, 0);
        len = 1;
    };

    explicit BigInt(size_t capacity, long long from) {
        digits.resize(capacity, 0);

        size_t i = 0;

        while (from > 0) {
            digits[i] = (int)(from % 10);
            from /= 10;
            i++;
        }

        len = i;

        if (len == 0) {
            len = 1;
        }
    }

    explicit BigInt(size_t capacity, std::string const &from) {
        size_t valid_len = from.size();

        for (size_t i = 0; i < from.size(); i++) {
            if (!std::isalnum(from[i])) {
                valid_len = i;
                break;
            }
        }

        digits.resize(capacity, 0);

        for (size_t i = 0; i < valid_len; i++) {
            digits[i] = from[from.size() - i - 1] - '0';
        }

        len = valid_len;

        if (len == 0) {
            len = 1;
        }
    }

    friend BigInt operator+(BigInt const &left, BigInt const &right) {
        BigInt result{std::max(left.digits.size(), right.digits.size())};

        size_t new_len = 0;

        if (left.len > right.len) {
            std::copy_n(std::begin(left.digits), left.len,
                        std::begin(result.digits));
            for (size_t i = 0; i < right.len; i++) {
                result.digits[i] += right.digits[i];
            }

            new_len = left.len;
        } else {
            std::copy_n(std::begin(right.digits), right.len,
                        std::begin(result.digits));

            for (size_t i = 0; i < left.len; i++) {
                result.digits[i] += left.digits[i];
            }

            new_len = right.len;
        }

        int carry = 0;
        for (size_t i = 0; i < result.digits.size(); i++) {
            result.digits[i] += carry;
            carry = result.digits[i] / 10;
            result.digits[i] %= 10;

            if (i >= new_len && carry == 0 && result.digits[i] == 0) {
                new_len = i;
                break;
            }
        }

        result.len = new_len;

        if (result.len == 0) {
            result.len = 1;
        }
        return result;
    }

    friend BigInt operator-(BigInt const &left, BigInt const &right) {
        BigInt result{left.digits.capacity()};

        std::copy_n(std::begin(left.digits), left.len,
                    std::begin(result.digits));

        for (size_t i = 0; i < left.len; i++) {
            result.digits[i] -= right.digits[i];
        }

        int borrow = 0;

        for (size_t i = 0; i < left.len; i++) {
            result.digits[i] -= borrow;

            if (result.digits[i] < 0) {
                borrow = -result.digits[i] / 10 + 1;
                result.digits[i] += borrow * 10;
            }
        }

        for (size_t i = 0; i < left.len; i++) {
            size_t idx = (left.len - i - 1);

            if (result.digits[i] != 0) {
                result.len = idx + 1;
                break;
            }
        }

        if (result.len == 0) {
            result.len = 1;
        }

        return result;
    }

    friend BigInt operator*(BigInt const &left, BigInt const &right) {
        BigInt result{std::max(left.digits.size(), right.digits.size())};

        for (size_t i = 0; i < left.len; i++) {
            for (size_t j = 0; j < right.len; j++) {
                result.digits[i + j] += left.digits[i] * right.digits[j];
            }
        }

        int carry = 0;
        for (size_t i = 0; i < result.digits.size(); i++) {
            result.digits[i] += carry;
            carry = result.digits[i] / 10;
            result.digits[i] %= 10;
        }

        result.len = 1;

        for (size_t i = 0; i < (left.len + right.len + 1); i++) {
            size_t idx = left.len + right.len - i;

            if (result.digits[idx] != 0) {
                result.len = idx + 1;
                break;
            }
        }

        return result;
    }

    std::string to_string() {
        std::string s;

        s.resize(len, '0');

        for (size_t i = 0; i < len; i++) {
            s[len - i - 1] = (char)digits[i] + '0';
        }

        return s;
    }
};

int main(void) {
    long long a, b;
    a = b = 0;

    std::scanf("%lld%lld", &a, &b);

    BigInt b_a{1000, a};
    BigInt b_b{1000, b};

    std::printf("a + b: %s\n", (b_a + b_b).to_string().data());
    std::printf("a - b: %s\n", (b_a - b_b).to_string().data());
    std::printf("a * b: %s\n", (b_a * b_b).to_string().data());

    return 0;
}