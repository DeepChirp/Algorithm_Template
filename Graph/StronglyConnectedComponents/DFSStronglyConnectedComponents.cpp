// 求强连通分量
// Verdict: https://www.luogu.com.cn/record/194487781

#include <algorithm>
#include <cstdio>
#include <vector>

size_t dfs_time;
std::vector<int> visited;
std::vector<size_t> finish_order;
std::vector<std::vector<size_t>> graph;
std::vector<std::vector<size_t>> reversed_graph;
std::vector<std::vector<size_t>> scc_by_node;

void dfs_pass1();
void dfs_visit_pass1(size_t source_node);
void dfs_pass2();
void dfs_visit_pass2(size_t source_node, size_t tree_root);
void generate_reverse_graph();

int main(void) {
    size_t n, m;
    n = m = 0;

    std::scanf("%zu%zu", &n, &m);

    graph.resize(n + 1);

    for (size_t i = 0; i < m; i++) {
        size_t from_node, to_node;
        from_node = to_node = 0;

        std::scanf("%zu%zu", &from_node, &to_node);

        graph[from_node].push_back(to_node);
    }

    generate_reverse_graph();
    dfs_pass1();
    dfs_pass2();

    for (size_t i = 1; i <= n; i++) {
        for (size_t scc_node : scc_by_node[i]) {
            std::printf("%zu ", scc_node);
        }
        if (!scc_by_node[i].empty()) {
            std::putchar('\n');
        }
    }

    return 0;
}

void dfs_pass1() {
    dfs_time = 0;
    visited.assign(graph.size(), 0);
    finish_order.clear();

    for (size_t i = 0; i < graph.size(); i++) {
        if (visited[i] == 0) {
            dfs_visit_pass1(i);
        }
    }

    std::reverse(std::begin(finish_order), std::end(finish_order));
}

void dfs_visit_pass1(size_t source_node) {
    dfs_time++;
    visited[source_node] = 1;

    for (size_t adjacent_node : graph[source_node]) {
        if (visited[adjacent_node] == 0) {
            dfs_visit_pass1(adjacent_node);
        }
    }

    dfs_time++;

    finish_order.push_back(source_node);
}

void dfs_pass2() {
    visited.assign(graph.size(), 0);
    scc_by_node.resize(graph.size());

    for (size_t node : finish_order) {
        if (visited[node] == 0) {
            dfs_visit_pass2(node, node);
        }
    }
}

void dfs_visit_pass2(size_t source_node, size_t tree_root) {
    visited[source_node] = 1;
    scc_by_node[tree_root].push_back(source_node);

    for (size_t adjacent_node : reversed_graph[source_node]) {
        if (visited[adjacent_node] == 0) {
            dfs_visit_pass2(adjacent_node, tree_root);
        }
    }
}

void generate_reverse_graph() {
    reversed_graph.resize(graph.size());

    for (size_t i = 1; i < graph.size(); i++) {
        for (size_t adjacent_node : graph[i]) {
            reversed_graph[adjacent_node].push_back(i);
        }
    }
}
