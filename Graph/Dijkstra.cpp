/*
 * name: Dijkstra 算法
 * description:
 *
 * Dijkstra 算法是一种用于计算图中的单源最短路径的算法。Dijkstra
 * 算法的主要思想是从起点开始，逐步扩展到距离起点更远的顶点，直到到达终点。
 *
 * Dijkstra 算法的基本思想是贪心算法。它维护一个数组 distance，其中 distance[u]
 * 表示从起点到顶点 u 的最短路径的长度。在每一步中，Dijkstra
 * 算法选择一个未标记的顶点 u，使得 distance[u] 最小，并标记顶点
 * u。然后，对于所有与顶点 u 相邻的顶点 v，更新 distance[v] = min(distance[v],
 * distance[u] + weight(u, v))。
 *
 * Verdict: P3371: https://www.luogu.com.cn/record/194010420
 *
 * Verdict: P4779: https://www.luogu.com.cn/record/194008908
 *
 */

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

#define MY_INFINITY (1LL << 60)

typedef long long i64;

struct Edge {
    size_t to;
    i64 weight;

    explicit constexpr Edge(size_t to_, i64 weight_)
        : to{to_}, weight{weight_} {}
};

struct NodeInfo {
    size_t node;
    i64 distance;

    friend bool operator<(NodeInfo const &left, NodeInfo const &right) {
        return left.distance > right.distance;
    }

    explicit constexpr NodeInfo(size_t node_, i64 distance_)
        : node{node_}, distance{distance_} {}
};

std::vector<i64> dijkstra(std::vector<std::vector<Edge>> const &node_to_edges,
                          size_t source_node) {
    std::vector<i64> result;
    std::vector<int> visited;

    result.resize(node_to_edges.size(), MY_INFINITY);
    visited.resize(node_to_edges.size(), 0);

    std::priority_queue<NodeInfo> node_queue;

    result[source_node] = 0;
    node_queue.emplace(source_node, 0);

    while (!node_queue.empty()) {
        NodeInfo current_node_info = node_queue.top();

        node_queue.pop();

        if (visited[current_node_info.node] == 1) {
            continue;
        }
        for (Edge const &edge : node_to_edges[current_node_info.node]) {
            size_t other_node = edge.to;

            if (result[other_node] >
                result[current_node_info.node] + edge.weight) {
                result[other_node] =
                    result[current_node_info.node] + edge.weight;
                node_queue.emplace(other_node, result[other_node]);
            }
        }

        visited[current_node_info.node] = 1;
    }

    return result;
}
