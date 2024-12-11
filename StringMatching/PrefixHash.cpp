/*
 * name: 字符串哈希（前缀）
 * description:
 *
 * 可用于快速比较字符串/字符串前缀/字符串后缀是否相同
 *
 * Verdict：E6 E
 */

#include <string>
#include <vector>

#define MOD 100000000000031ULL

typedef unsigned long long u64;

std::vector<u64> string_prefix_hash(std::string const &str);
u64 string_hash(std::string const &str);

struct StringPack {
    std::string str;
    std::vector<u64> prefix_hash;

    explicit StringPack(std::string const &_str) {
        str = _str;
        prefix_hash = string_prefix_hash(_str);
    }
};

// result[i] 表示字符串长度为i的前缀的hash，result[0]恒为0
std::vector<u64> string_prefix_hash(std::string const &str) {
    std::vector<u64> result;

    result.reserve(str.size() + 1);

    u64 prefix_sum = 0;

    result.push_back(prefix_sum);

    for (size_t prefix_len = 1; prefix_len <= str.size(); prefix_len++) {
        prefix_sum = (prefix_sum * 128 + (u64)str[prefix_len - 1]) % MOD;
        result.push_back(prefix_sum);
    }

    return result;
}

// 整个字符串的hash
u64 string_hash(std::string const &str) {
    u64 prefix_sum = 0;

    for (size_t prefix_len = 1; prefix_len <= str.size(); prefix_len++) {
        prefix_sum = (prefix_sum * 128 + (u64)str[prefix_len - 1]) % MOD;
    }

    return prefix_sum;
}