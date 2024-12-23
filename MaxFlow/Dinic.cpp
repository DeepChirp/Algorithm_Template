/*
 * name: Dinic 算法
 * description:
 *
 * Dinic 算法是求解网络流问题的一种算法，是 Edmonds-Karp 算法的优化版本。
 *
 * 时间复杂度：$O(V^2 * E)$。
 *
 * Dinic 算法的基本思想是通过多次广度优先搜索（BFS）找到层次图，然后通过多次深度优先搜索（DFS）找到增广路径，最终求解出最大流。
 */

#include <cstdint>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int MAX_NUM = 105;
const int64_t INF = 1e18;

struct Edge {
    int to, rev;
    int64_t cap;
};

vector<Edge> G[MAX_NUM];
int level[MAX_NUM];
int iter[MAX_NUM];

void add_edge(int from, int to, int64_t cap) {
    G[from].push_back({to, (int)G[to].size(), cap});
    G[to].push_back({from, (int)G[from].size() - 1, 0});
}

auto bfs(int s, int t) -> bool {
    memset(level, -1, sizeof(level));
    queue<int> que;
    level[s] = 0;
    que.push(s);
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        for (const auto &e : G[v]) {
            if (e.cap > 0 && level[e.to] < 0) {
                level[e.to] = level[v] + 1;
                que.push(e.to);
            }
        }
    }
    return level[t] != -1;
}

auto dfs(int v, int t, int64_t f) -> int64_t {
    if (v == t) {
        return f;
    }
    for (int &i = iter[v]; i < G[v].size(); ++i) {
        Edge &e = G[v][i];
        if (e.cap > 0 && level[v] < level[e.to]) {
            int64_t d = dfs(e.to, t, min(f, e.cap));
            if (d > 0) {
                e.cap -= d;
                G[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}

auto max_flow(int s, int t) -> int64_t {
    int64_t flow = 0;
    while (bfs(s, t)) {
        memset(iter, 0, sizeof(iter));
        int64_t f;
        while ((f = dfs(s, t, INF)) > 0) {
            flow += f;
        }
    }
    return flow;
}

auto main() -> int {
    // n: 点数 m: 边数 s: 源点 t: 汇点
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    for (int i = 1; i <= n; ++i) {
        G[i].clear();
    }
    for (int i = 0; i < m; ++i) {
        int u, v;
        int64_t w;
        cin >> u >> v >> w;
        add_edge(u, v, w);
    }
    cout << max_flow(s, t) << '\n';
    return 0;
}
