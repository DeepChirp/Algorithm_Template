#include <algorithm>
#include <cstdio>
#include <vector>

size_t dfs_time = 0;
std::vector<std::vector<size_t>> graph;

std::vector<int> visited;
std::vector<size_t> parent;
std::vector<size_t> discover_time;
std::vector<size_t> finish_time;
std::vector<size_t> topo_list;

void dfs_visit(size_t source_node) {
    dfs_time++;
    discover_time[source_node] = dfs_time;
    visited[source_node] = 1;

    for (size_t adjacent_node : graph[source_node]) {
        if (visited[adjacent_node] == 0) {
            parent[adjacent_node] = source_node;
            discover_time[adjacent_node] = dfs_time;
            dfs_visit(adjacent_node);
        }
    }

    dfs_time++;
    finish_time[source_node] = dfs_time;

    topo_list.push_back(source_node);
}

void dfs_graph() {
    dfs_time = 0;

    visited.assign(graph.size(), 0);
    parent.assign(graph.size(), 0);
    discover_time.assign(graph.size(), 0);
    finish_time.assign(graph.size(), 0);
    topo_list.resize(0);

    for (size_t i = 1; i < graph.size(); i++) {
        if (visited[i] == 0) {
            dfs_visit(i);
        }
    }

    std::reverse(std::begin(topo_list), std::end(topo_list));
}

int main(void) {
    size_t N = 0;

    std::scanf("%zu", &N);

    graph.resize(N + 1);

    for (size_t i = 1; i <= N; i++) {
        size_t a = 0;

        while (std::scanf("%zu", &a) == 1) {
            if (a == 0) {
                break;
            }

            graph[i].push_back(a);
        }
    }

    dfs_graph();

    for (size_t node : topo_list) {
        std::printf("%zu ", node);
    }

    std::putchar('\n');

    return 0;
}