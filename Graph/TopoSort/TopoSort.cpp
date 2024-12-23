// Verdict: P1347: https://www.luogu.com.cn/record/192509279
#include <queue>
#include <vector>

struct TopoResult {
    int result_code;
    std::vector<size_t> result;
};

// 0 下标
// Sorted sequence cannot be
// determined，表示拓扑排序不唯一（条件：任意时刻入度为0的顶点的集合的元素个数大于1）

TopoResult topo_sort(std::vector<std::vector<size_t>> const &graph) {
    // 0 -> Sorted sequence determined
    // 1 -> Inconsistency found
    // 2 -> Sorted sequence cannot be determined.
    int result_code = 0;
    std::vector<size_t> result;
    result.reserve(graph.size());

    std::vector<size_t> in_count;
    in_count.resize(graph.size(), 0);

    for (std::vector<size_t> const &out_list : graph) {
        for (size_t out_node : out_list) {
            in_count[out_node]++;
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

        for (size_t other_node : graph[current_node]) {
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