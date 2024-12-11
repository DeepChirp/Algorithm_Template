/*
 * name: Prim 算法
 * description:
 *
 * Prim 算法是一种求解加权连通图的最小生成树的算法。Prim 算法是通过不断地选择边来构造最小生成树。
 *
 * 与 Kruskal 算法不同，Prim 算法是从一个顶点开始，逐步长出最小生成树的每一条边，直到所有的顶点都被包含在最小生成树中。
 */

#include <cstdint>
#include <iostream>
#include <vector>

// 定义边的结构体
struct Edge
{
    uint64_t from;   // 起点
    uint64_t to;     // 终点
    uint64_t weight; // 权重
};

using namespace std;

auto main() -> int
{
    // 节点数：n；边数：m
    uint64_t n, m;
    cin >> n >> m;

    // 创建图的邻接表表示，大小为 (2 * m) + 1
    vector<vector<Edge>> graph((2 * m) + 1);
    for (size_t i = 0; i < m; i++)
    {
        // 边的起点：u；终点：v；权重：w
        uint64_t u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({u, v, w}); // 添加边 u->v
        graph[v].push_back({v, u, w}); // 添加边 v->u，因图为无向图
    }

    vector<bool> visited(n + 1);              // 标记节点是否已被访问
    vector<uint64_t> dist(n + 1, UINT64_MAX); // 存储节点到生成树的最小权重
    dist[1] = 0;                              // 从节点 1 开始
    uint64_t result = 0;                      // 最小生成树的总权重

    // Prim 算法主循环
    for (size_t i = 1; i <= n; i++)
    {
        uint64_t u = 0;
        // 在未访问的节点中找到权值最小的节点 u
        for (size_t j = 1; j <= n; j++)
        {
            if (!visited[j] && (u == 0 || dist[j] < dist[u]))
            {
                u = j;
            }
        }
        visited[u] = true; // 标记节点 u 为已访问
        result += dist[u]; // 将权值累加到结果中
        // 更新与节点 u 相邻的未访问节点的权值
        for (auto &j : graph[u])
        {
            uint64_t v = j.to;
            uint64_t w = j.weight;
            if (!visited[v] && w < dist[v])
            {
                dist[v] = w; // 更新节点 v 的最小权值
            }
        }
    }
    cout << result << '\n'; // 输出最小生成树的总权重

    return 0;
}
