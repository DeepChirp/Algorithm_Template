/*
 * name: Prim 算法
 * description:
 *
 * Prim 算法，用于计算最小生成树。
 *
 * 时间复杂度：$O((V + E) \log V)$。
 *
 * Verdict: P3366: https://www.luogu.com.cn/record/195050793
 */
#include <cstdbool>
#include <cstdint>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

struct Edge {
    uint64_t to, weight, next;
};

struct State {
    uint64_t node, distance;
    friend bool operator>(const State &a, const State &b) {
        return a.distance > b.distance;
    }
};

uint64_t num_nodes, num_edges, edge_count;
vector<uint64_t> min_distance;
vector<bool> visited;
vector<uint64_t> head;
// head.resize(num_nodes + 1);
vector<Edge> edges;
// edges.resize(num_edges * 2 + 1);
uint64_t total_weight = 0, connected_nodes = 0;

priority_queue<State, vector<State>, greater<State>> pq;

void add_edge(uint64_t from, uint64_t to, uint64_t weight) {
    edges[++edge_count] = Edge{to, weight, head[from]};
    head[from] = edge_count;
}

void prim() {
    min_distance.resize(num_nodes + 1, numeric_limits<uint64_t>::max());
    min_distance[1] = 0;
    visited.resize(num_nodes + 1);
    pq.push({1, 0});
    while (!pq.empty()) {
        if (connected_nodes >= num_nodes) {
            break;
        }
        uint64_t current_node = pq.top().node,
                 current_distance = pq.top().distance;
        pq.pop();
        if (visited[current_node]) {
            continue;
        }
        visited[current_node] = true;
        connected_nodes++;
        total_weight += current_distance;
        for (uint64_t i = head[current_node]; i != 0; i = edges[i].next) {
            uint64_t next_node = edges[i].to, edge_weight = edges[i].weight;
            if (edge_weight < min_distance[next_node]) {
                min_distance[next_node] = edge_weight;
                pq.push({next_node, edge_weight});
            }
        }
    }
}