/*
 * name: 快速排序
 *
 * description:
 *
 * 时间复杂度：
 * 最坏情况：$O(n^2)$ / 平均情况：$O(n \log n)$ / 最好情况：$O(n \log n)$
 */

// 过不了极端数据
// 对于极端数据，请使用三路快速排序

#include <cstdio>
#include <random>
#include <vector>

typedef long long i64;

i64 partition(std::vector<i64> &list, i64 p, i64 r);
void do_quick_sort(std::vector<i64> &list, i64 p, i64 r);
void quick_sort(std::vector<i64> &list);
void random_permutation(std::vector<i64> &list);

int main(void) {
    size_t n = 0;
    std::scanf("%zu", &n);

    std::vector<i64> num_list;

    num_list.reserve(n);

    for (size_t i = 0; i < n; i++) {
        i64 num = 0;
        std::scanf("%lld", &num);
        num_list.push_back(num);
    }

    quick_sort(num_list);

    for (i64 num : num_list) {
        std::printf("%lld ", num);
    }

    std::putchar('\n');

    return 0;
}

// partition range [p, r]
i64 partition(std::vector<i64> &list, i64 p, i64 r) {
    i64 pivot = list[(size_t)r];

    i64 i = p - 1;

    for (i64 j = p; j <= r - 1; j++) {
        if (list[(size_t)j] <= pivot) {
            i++;

            std::swap(list[(size_t)i], list[(size_t)j]);
        }
    }

    std::swap(list[(size_t)i + 1], list[(size_t)r]);

    return i + 1;
}

void do_quick_sort(std::vector<i64> &list, i64 p, i64 r) {
    if (p < r) {
        i64 q = partition(list, p, r);

        do_quick_sort(list, p, q - 1);
        do_quick_sort(list, q + 1, r);
    }
}

void quick_sort(std::vector<i64> &list) {
    if (list.empty()) {
        return;
    }
    random_permutation(list);
    do_quick_sort(list, 0, (i64)(list.size() - 1));
}

void random_permutation(std::vector<i64> &list) {
    auto const seed = 84841984;

    std::mt19937 urbg{seed};

    for (size_t i = 0; i < list.size(); i++) {
        std::uniform_int_distribution<size_t> distr{i, list.size() - 1};

        std::swap(list[i], list[distr(urbg)]);
    }
}