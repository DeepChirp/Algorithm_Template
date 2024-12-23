// Verdict: E4 A
// Verdict: P3376: https://www.luogu.com.cn/record/194584006
// O(V(E^2))
// 处理了反平行边、重边、自环
// 反平行边：新建节点
// 重边：容量相加
// 自环：直接移除

#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>

#define MAX_NODE 2005
#define INFINITY (1LL << 60)

typedef long long i64;

i64 residual_graph[MAX_NODE][MAX_NODE];

struct Edge {
    size_t to;
    i64 weight;

    explicit constexpr Edge(size_t to_, i64 weight_)
        : to{to_}, weight{weight_} {}
};

class EK {
  public:
    size_t node_count;

  private:
    std::vector<std::vector<size_t>> node_to_edges;

  public:
    explicit EK(std::vector<std::vector<Edge>> const &graph) {
        std::memset(residual_graph, 0, sizeof(residual_graph));
        node_count = graph.size() - 1;
        node_to_edges.resize(graph.size());

        for (size_t from_node = 0; from_node < graph.size(); from_node++) {
            for (Edge const &edge : graph[from_node]) {
                size_t to_node = edge.to;
                i64 weight = edge.weight;

                // antiparallel edge exists
                // parallel edges are allowed
                if (residual_graph[to_node][from_node] != 0) {
                    node_count++;

                    residual_graph[from_node][node_count] += weight;
                    residual_graph[node_count][to_node] += weight;

                    if (node_count > node_to_edges.size() - 1) {
                        node_to_edges.resize(2 * node_count);
                    }

                    // 注意须在node_to_edges（对应残余图）中建立反向边
                    node_to_edges[from_node].push_back(node_count);
                    node_to_edges[node_count].push_back(from_node);
                    node_to_edges[node_count].push_back(to_node);
                    node_to_edges[to_node].push_back(node_count);
                } else {
                    residual_graph[from_node][to_node] += weight;
                    node_to_edges[from_node].push_back(to_node);
                    node_to_edges[to_node].push_back(from_node);
                }
            }
        }
        // remove self loop

        for (size_t i = 0; i <= node_count; i++) {
            residual_graph[i][i] = 0;
        }
    }

    i64 max_flow(size_t source_node, size_t target_node) {
        i64 max_flow = 0;

        while (true) {
            i64 augment = rg_bfs(source_node, target_node);

            // std::printf("augment: %lld\n", augment);

            if (augment == 0) {
                break;
            }

            max_flow += augment;
        }

        return max_flow;
    }

  private:
    i64 rg_bfs(size_t from_node, size_t to_node) {
        std::vector<int> visited;

        visited.resize(node_count + 1, 0);

        std::queue<size_t> node_queue;
        std::vector<size_t> parent;

        parent.resize(node_count + 1, 0);

        node_queue.push(from_node);

        visited[from_node] = 1;

        while (!node_queue.empty()) {
            size_t current_node = node_queue.front();

            node_queue.pop();

            if (current_node == to_node) {
                break;
            }

            for (size_t next_node : node_to_edges[current_node]) {
                if (residual_graph[current_node][next_node] > 0) {
                    if (visited[next_node] == 0) {
                        visited[next_node] = 1;
                        parent[next_node] = current_node;
                        node_queue.push(next_node);
                    }
                }
            }
        }

        if (!visited[to_node]) {
            return 0;
        }

        size_t current_node = to_node;
        i64 augment = INFINITY;

        while (current_node != from_node) {
            augment = std::min(
                augment, residual_graph[parent[current_node]][current_node]);
            current_node = parent[current_node];
        }

        current_node = to_node;

        while (current_node != from_node) {
            residual_graph[parent[current_node]][current_node] -= augment;
            residual_graph[current_node][parent[current_node]] += augment;

            current_node = parent[current_node];
        }

        return augment;
    }
};

int main() {
    int T = 0;
    std::scanf("%d", &T);

    for (int id = 0; id < T; id++) {

        size_t node_count, edge_count, source_node, target_node;

        std::scanf("%zu%zu%zu%zu", &node_count, &edge_count, &source_node,
                   &target_node);

        std::vector<std::vector<Edge>> graph;

        graph.resize(node_count + 1);

        for (size_t i = 0; i < edge_count; i++) {
            size_t from_node, to_node;
            from_node = to_node = 0;
            i64 weight = 0;

            std::scanf("%zu%zu%lld", &from_node, &to_node, &weight);

            graph[from_node].emplace_back(to_node, weight);
        }

        EK ek{graph};

        std::printf("%lld\n", ek.max_flow(source_node, target_node));
    }
}
