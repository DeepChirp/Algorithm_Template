/*
 * name: 并查集
 * description:
 *
 * 并查集是一种用于管理元素所属集合的数据结构，实现为一个森林，其中每棵树表示一个集合，树中的节点表示对应集合中的元素。
 * 若大小为$size$，可用节点编号为$0$到$size - 1$（零下标）
 *
 * 每个操作的平均时间几乎为$O(1)$
 */

#include <cstddef>
#include <numeric>
#include <vector>

using std::size_t;

struct DisjointSet {
    // 节点i的父亲
    std::vector<size_t> set;

    // 以节点i为树根的子树的大小
    std::vector<size_t> set_size;

    explicit DisjointSet(size_t size) {
        set.resize(size);
        set_size.resize(size, 1);
        std::iota(std::begin(set), std::end(set), 0);
    }

    // 查询对应元素的根节点（+路径压缩）
    size_t find(size_t x) {
        std::vector<size_t> path(16);

        while (set[x] != x) {
            path.push_back(x);
            x = set[x];
        };

        for (size_t node : path) {
            set[node] = x;
        }

        return x;
    }

    // 合并两个元素所属的集合（+启发式合并）
    void unite(size_t x, size_t y) {
        size_t x_root = find(x);
        size_t y_root = find(y);

        if (x_root == y_root) {
            return;
        }

        if (set_size[x_root] < set_size[y_root]) {
            set[x_root] = y_root;
            set_size[y_root] += set_size[x_root];
        } else {
            set[y_root] = x_root;
            set_size[x_root] += set_size[y_root];
        }
    }
};