// 最长不增子序列，动态规划+二分，O(NlogN)
// 若要求最长不减子序列，将数组中所有元素取负即可
// 若要求最长递减子序列（即，不能出现相等的情况），在调用二分查找时将val + 1即可
// 注意最长不增子序列的长度为dp中的最大值
// Verdict: B3637: https://www.luogu.com.cn/record/195490309
// Verdict: P1439: https://www.luogu.com.cn/record/195524254
// Verdict: E3.H
#include <vector>

#define MY_INFINITY (1LL << 60)

typedef long long i64;
i64 get_length(std::vector<i64> const &list, i64 left, i64 right, i64 val);
std::vector<i64> calc_dp(std::vector<i64> const &num_list);

std::vector<i64> calc_dp(std::vector<i64> const &num_list) {
    std::vector<i64> dp;
    dp.resize(num_list.size(), 0);

    std::vector<i64> max_last_element_by_length;

    max_last_element_by_length.resize(num_list.size() + 1, -MY_INFINITY);

    max_last_element_by_length[0] = MY_INFINITY;

    dp[0] = 1;
    max_last_element_by_length[1] = num_list[0];

    for (size_t i = 1; i < num_list.size(); i++) {
        i64 prev_max_length =
            get_length(max_last_element_by_length, 1, (i64)i, num_list[i]);

        dp[i] = prev_max_length + 1;

        if (max_last_element_by_length[(size_t)dp[i]] < num_list[i]) {
            max_last_element_by_length[(size_t)dp[i]] = num_list[i];
        }
    }

    return dp;
}

i64 get_length(std::vector<i64> const &list, i64 left, i64 right, i64 val) {
    while (left <= right) {
        i64 mid = (left + right) / 2;

        if (list[(size_t)mid] < val) {
            right = mid - 1;
        } else if (list[(size_t)mid] > val) {
            left = mid + 1;
        } else {
            left = mid + 1;
        }
    }

    return right;
}