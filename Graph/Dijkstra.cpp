/*
 * name: Dijkstra 算法
 * description:
 *
 * Dijkstra 算法是一种用于计算图中的单源最短路径的算法。Dijkstra 算法的主要思想是从起点开始，逐步扩展到距离起点更远的顶点，直到到达终点。
 *
 * Dijkstra 算法的基本思想是贪心算法。它维护一个数组 distance，其中 distance[u] 表示从起点到顶点 u 的最短路径的长度。在每一步中，Dijkstra 算法选择一个未标记的顶点 u，使得 distance[u] 最小，并标记顶点 u。然后，对于所有与顶点 u 相邻的顶点 v，更新 distance[v] = min(distance[v], distance[u] + weight(u, v))。
 */

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

struct Edge
{
    uint64_t to;
    uint64_t weight;
};

namespace
{
auto dijkstra(uint64_t start, const vector<vector<Edge>> &graph) -> vector<uint64_t>
{
    // 初始化距离数组，所有距离初始化为无穷大
    vector<uint64_t> distance(graph.size(), UINT64_MAX);
    distance[start] = 0; // 起点到自己的距离为0

    // 优先队列，保存(距离,节点)对，按距离从小到大排序
    priority_queue<pair<uint64_t, uint64_t>,
                   vector<pair<uint64_t, uint64_t>>,
                   greater<>>
        priority_queue;
    priority_queue.emplace(0, start);

    while (!priority_queue.empty())
    {
        // 取出当前距离最小的节点
        auto [current_distance, u] = priority_queue.top();
        priority_queue.pop();

        // 如果取出的距离大于已知距离，说明这是一个过时的记录，跳过
        if (current_distance > distance[u])
        {
            continue;
        }

        // 遍历当前节点的所有出边，尝试更新到达相邻节点的最短距离
        for (const auto &edge : graph[u])
        {
            // 如果通过当前节点u可以得到一条到edge.to的更短路径
            if (distance[edge.to] > distance[u] + edge.weight)
            {
                // 更新最短距离
                distance[edge.to] = distance[u] + edge.weight;
                // 将新的距离和节点加入优先队列
                priority_queue.emplace(distance[edge.to], edge.to);
            }
        }
    }
    return distance; // 返回所有最短距离
}
} // namespace

auto main() -> int
{
    uint64_t n, m, k;
    cin >> n >> m >> k;
    vector<vector<Edge>> graph(n + 1);
    vector<uint64_t> mandatory_points(k);
    for (size_t j = 0; j < k; j++)
    {
        cin >> mandatory_points[j];
    }
    for (size_t j = 0; j < m; j++)
    {
        uint64_t from, to, weight;
        cin >> from >> to >> weight;
        graph[from].push_back({to, weight});
        graph[to].push_back({from, weight});
    }

    // 其中 distance_from_start 存储从起点到各个顶点的最短路径长度，distance_from_end 存储从终点到各个顶点的最短路径长度
    vector<uint64_t> distance_from_start = dijkstra(1, graph);
    vector<uint64_t> distance_from_end = dijkstra(n, graph);

    uint64_t min_distance = UINT64_MAX;
    for (const auto &p : mandatory_points)
    {
        if (distance_from_start[p] != UINT64_MAX && distance_from_end[p] != UINT64_MAX)
        {
            uint64_t total_distance = distance_from_start[p] + distance_from_end[p];
            min_distance = min(min_distance, total_distance);
        }
    }

    if (min_distance == UINT64_MAX)
    {
        cout << -1 << '\n';
    }
    else
    {
        cout << min_distance << '\n';
    }

    return 0;
}
