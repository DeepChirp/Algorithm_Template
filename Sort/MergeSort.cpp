/*
 * name: 归并排序
 *
 * description:
 *
 * 时间复杂度：
 * 最坏情况：$O(n \log n)$ / 平均情况：$O(n \log n)$ / 最好情况：$O(n \log n)$
 *
 * 空间复杂度：$O(n)$
 */

#include <algorithm>
#include <cstdio>
#include <vector>

typedef long long i64;

void merge(std::vector<i64> &arr, size_t p, size_t q, size_t r);
void do_merge_sort(std::vector<i64> &arr, size_t p, size_t r);
void merge_sort(std::vector<i64> &arr);

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

    merge_sort(num_list);

    for (i64 num : num_list) {
        std::printf("%lld ", num);
    }

    std::putchar('\n');

    return 0;
}

// merge sorted [p, q] and [q + 1, r]
void merge(std::vector<i64> &arr, size_t p, size_t q, size_t r) {
    std::vector<i64> aux;
    aux.reserve(r - p + 1);

    size_t i = p;
    size_t j = q + 1;

    while ((i <= q) && (j <= r)) {
        if (arr[i] <= arr[j]) {
            aux.push_back(arr[i]);
            i++;
        } else {
            aux.push_back(arr[j]);
            j++;
        }
    }

    while (i <= q) {
        aux.push_back(arr[i]);
        i++;
    }

    while (j <= r) {
        aux.push_back(arr[j]);
        j++;
    }

    std::copy(std::begin(aux), std::end(aux), std::begin(arr) + (long)p);
}

void do_merge_sort(std::vector<i64> &arr, size_t p, size_t r) {
    if (p == r) {
        return;
    }

    size_t q = p + (r - p) / 2;

    do_merge_sort(arr, p, q);
    do_merge_sort(arr, q + 1, r);

    merge(arr, p, q, r);
}

void merge_sort(std::vector<i64> &arr) {
    if (arr.empty() || arr.size() == 1) {
        return;
    }

    do_merge_sort(arr, 0, arr.size() - 1);
}