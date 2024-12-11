/*
 * name: Kruskal 算法
 * description:
 *
 * Kruskal 算法，用于计算最小生成树。
 *
 * Verdict: P3366: https://www.luogu.com.cn/record/189763566
 */

#include <algorithm>
#include <cstdio>
#include <numeric>
#include <vector>

struct DisjointSet {
    std::vector<size_t> set;
    std::vector<size_t> set_size;

    explicit DisjointSet(size_t size) {
        set.resize(size);
        set_size.resize(size, 1);
        std::iota(std::begin(set), std::end(set), 0);
    }

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

// 1 下标

struct Edge {
    size_t from;
    size_t to;
    int weight;

    explicit constexpr Edge(size_t from_, size_t to_, int weight_)
        : from{from_}, to{to_}, weight{weight_} {};
};

std::vector<Edge> Kruskal(std::vector<Edge> &edge_list, size_t node_count);

int main(void) {
    size_t node_count, edge_count;
    node_count = edge_count = 0;

    std::scanf("%zu%zu", &node_count, &edge_count);

    std::vector<Edge> edge_list;

    edge_list.reserve(edge_count);

    for (size_t i = 0; i < edge_count; i++) {
        size_t from, to;
        from = to = 0;
        int weight = 0;

        std::scanf("%zu%zu%d", &from, &to, &weight);

        edge_list.emplace_back(from, to, weight);
    }

    std::vector<Edge> mst_edge_list = Kruskal(edge_list, node_count);

    if (mst_edge_list.size() < (node_count - 1)) {
        std::printf("orz\n");
    } else {
        int sum = std::accumulate(
            std::begin(mst_edge_list), std::end(mst_edge_list), 0,
            [](int left, Edge const &right) { return left + right.weight; });
        std::printf("%d\n", sum);
    }

    return 0;
}

std::vector<Edge> Kruskal(std::vector<Edge> &edge_list, size_t node_count) {
    std::sort(std::begin(edge_list), std::end(edge_list),
              [](Edge const &a, Edge const &b) { return a.weight < b.weight; });

    DisjointSet set{node_count + 1};

    std::vector<Edge> result;

    for (Edge const &edge : edge_list) {
        size_t from = edge.from;
        size_t to = edge.to;

        size_t from_root = set.find(from);
        size_t to_root = set.find(to);

        if (from_root != to_root) {
            result.push_back(edge);
            set.unite(from_root, to_root);
        }

        if (result.size() == (node_count - 1)) {
            break;
        }
    }

    return result;
}