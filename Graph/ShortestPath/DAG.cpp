// 警告：暂时未经验证！
// 其中，拓扑排序部分已经验证

#include <queue>
#include <vector>

#define MY_INFINITY (1LL << 61)

typedef long long i64;

struct Edge {
    size_t to;
    i64 weight;

    explicit constexpr Edge(size_t to_, i64 weight_)
        : to{to_}, weight{weight_} {};
};

struct TopoResult {
    int result_code;
    std::vector<size_t> result;
};

struct DistanceResult {
    std::vector<i64> shortest_distance;
    std::vector<size_t> parent;
};

TopoResult topo_sort(std::vector<std::vector<Edge>> const &graph);
DistanceResult DAGShortestPath(std::vector<std::vector<Edge>> const &graph,
                               size_t source_node);

// Sorted sequence cannot be
// determined，表示拓扑排序不唯一（条件：任意时刻入度为0的顶点的集合的元素个数大于1）

TopoResult topo_sort(std::vector<std::vector<Edge>> const &graph) {
    // 0 -> Sorted sequence determined
    // 1 -> Inconsistency found
    // 2 -> Sorted sequence cannot be determined.
    int result_code = 0;
    std::vector<size_t> result;
    result.reserve(graph.size());

    std::vector<size_t> in_count;
    in_count.resize(graph.size(), 0);

    for (std::vector<Edge> const &out_list : graph) {
        for (Edge const &edge : out_list) {
            in_count[edge.to]++;
        }
    }

    std::queue<size_t> node_queue;

    for (size_t i = 0; i < graph.size(); i++) {
        if (in_count[i] == 0) {
            node_queue.push(i);
        }
    }

    if (node_queue.size() > 1) {
        result_code = 2;
    }

    while (!node_queue.empty()) {
        if (node_queue.size() > 1) {
            result_code = 2;
        }
        size_t current_node = node_queue.front();
        node_queue.pop();

        result.push_back(current_node);

        for (Edge const &edge : graph[current_node]) {
            size_t other_node = edge.to;

            if (in_count[other_node] == 1) {
                node_queue.push(other_node);
            }

            in_count[other_node]--;
        }
    }

    if (result.size() < graph.size()) {
        result_code = 1;
    }

    TopoResult r = {result_code, result};

    return r;
}

DistanceResult DAGShortestPath(std::vector<std::vector<Edge>> const &graph,
                               size_t source_node) {
    std::vector<i64> distance;
    std::vector<size_t> parent;

    TopoResult topo_result = topo_sort(graph);

    std::vector<size_t> topo_list = topo_result.result;

    distance.resize(graph.size(), MY_INFINITY);
    parent.resize(graph.size(), 0);

    distance[source_node] = 0;

    for (size_t from_node : topo_list) {
        for (Edge const &edge : graph[from_node]) {
            size_t to_node = edge.to;

            if (distance[from_node] + edge.weight < distance[to_node]) {
                distance[to_node] = distance[from_node] + edge.weight;
                parent[to_node] = from_node;
            }
        }
    }

    return {distance, parent};
}