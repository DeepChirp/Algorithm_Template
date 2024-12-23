// Verdict: C4 G
// Verdict: https://www.luogu.com.cn/record/194504643

#include <vector>

#define MY_INFINITY (1LL << 61)

typedef long long i64;

struct Edge {
    size_t to;
    i64 weight;

    explicit constexpr Edge(size_t to_, i64 weight_)
        : to{to_}, weight{weight_} {};
};

struct BFResult {
    bool valid;
    std::vector<i64> shortest_distance;
    std::vector<size_t> parent;
};

BFResult BF(std::vector<std::vector<Edge>> const &graph, size_t source_node) {
    bool valid = true;
    std::vector<i64> shortest_distance;
    std::vector<size_t> parent;

    shortest_distance.resize(graph.size(), MY_INFINITY);
    parent.resize(graph.size(), 0);

    shortest_distance[source_node] = 0;

    for (size_t i = 0; i < graph.size() - 1; i++) {
        for (size_t from_node = 1; from_node < graph.size(); from_node++) {
            for (Edge const &edge : graph[from_node]) {
                size_t to_node = edge.to;

                if (shortest_distance[from_node] + edge.weight <
                    shortest_distance[to_node]) {
                    shortest_distance[to_node] =
                        shortest_distance[from_node] + edge.weight;
                    parent[to_node] = from_node;
                }
            }
        }
    }

    for (size_t from_node = 1; from_node < graph.size(); from_node++) {
        for (Edge const &edge : graph[from_node]) {
            if (shortest_distance[from_node] + edge.weight <
                shortest_distance[edge.to]) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            break;
        }
    }

    return {valid, shortest_distance, parent};
}
