#include <vector>

typedef long long i64;

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

// 将改变list！
// nth从0开始

i64 do_nth_element(std::vector<i64> &list, i64 p, i64 r, i64 nth) {
    i64 pivot_idx = partition(list, p, r);

    if (pivot_idx == nth) {
        return list[(size_t)pivot_idx];
    } else if (pivot_idx > nth) {
        return do_nth_element(list, p, pivot_idx - 1, nth);
    } else {
        return do_nth_element(list, pivot_idx + 1, r, nth);
    }
}