# 算法模板

## StringMatching

### KMP，字符串有限状态机

KMP：
- `KMPMatcher(std::string const &pattern)`：创建以pattern为模式的字符串匹配“状态机”
- `match(std::string const &to_match)`：返回`to_match`中所有匹配位置的起始下标（0下标）
- `void generate_FA_transition_table(size_t delta[MAX_STR_LEN][ALPHABET_LEN])`：利用$\pi$数组生成字符串有限状态机的状态转移表


Verdict: P3375: https://www.luogu.com.cn/record/192756655
Verdict：E6 A, B, D

```cpp
#include <string>
#include <vector>

// 字符串有限状态机的状态转移表的长度以及字符集的长度
#define MAX_STR_LEN 305
#define ALPHABET_LEN 30

// 状态q表示匹配了pattern的前q个字符
class KMPMatcher {
  public:
    std::string pattern;
    std::vector<size_t> pi;

    explicit KMPMatcher(std::string const &pattern) {
        this->pattern = pattern;

        generate_pi();
    }

    // $O(M\Sigma)$
    // `delta[i][j]`表示若当前匹配了`i`个字符，下一个输入的字符为j（偏移量），则应当转移为匹配`delta[i][j]`个字符。
    // 注意：`需要使用`delta[i]['x' - 'a']`而非`delta[i]['x']`的方式访问
    // 注意：此处假设字母表为小写字母，若不是，需修改内层`for`循环的条件、`delta`二维数组的偏移量，`ALPHABET_LEN`
    void generate_FA_transition_table(size_t delta[MAX_STR_LEN][ALPHABET_LEN]) {
        for (size_t l = 0; l < pi.size(); l++) {
            for (size_t next_ch = 'a'; next_ch <= 'z'; next_ch++) {
                size_t q = l;
                while ((q > 0) && ((char)next_ch != pattern[q])) {
                    q = pi[q];
                }

                if ((char)next_ch == pattern[q]) {
                    delta[l][next_ch - 'a'] = q + 1;
                } else {
                    delta[l][next_ch - 'a'] = q;
                }
            }
        }
    }

    // $O(N)$
    std::vector<size_t> match(std::string const &to_match) {
        std::vector<size_t> matched_pos_list;

        size_t q = 0;

        for (size_t i = 0; i < to_match.size(); i++) {
            while (q > 0 && to_match[i] != pattern[q]) {
                q = pi[q];
            }

            // 0 下标，故已经匹配了q个字符的情况下，下一个比较的字符是pattern[q]
            if (to_match[i] == pattern[q]) {
                q++;
            }

            if (q == pattern.size()) {
                matched_pos_list.push_back(i + 1 - pattern.size());
                q = pi[q];
            }
        }

        return matched_pos_list;
    }

  private:
    // $O(M)$
    void generate_pi() {
        pi.resize(pattern.size() + 1, 0);

        size_t q = 0;

        for (size_t i = 1; i < pattern.size(); i++) {
            while (q > 0 && pattern[i] != pattern[q]) {
                q = pi[q];
            }

            if (pattern[i] == pattern[q]) {
                q++;
            }

            pi[i + 1] = q;
        }
    }
};
```

### 字符串哈希（前缀）

可用于快速比较字符串/字符串前缀/字符串后缀是否相同

Verdict：E6 E

```cpp
#include <string>
#include <vector>

#define MOD 100000000000031ULL

typedef unsigned long long u64;

std::vector<u64> string_prefix_hash(std::string const &str);
u64 string_hash(std::string const &str);

struct StringPack {
    std::string str;
    std::vector<u64> prefix_hash;

    explicit StringPack(std::string const &_str) {
        str = _str;
        prefix_hash = string_prefix_hash(_str);
    }
};

// result[i] 表示字符串长度为i的前缀的hash，result[0]恒为0
std::vector<u64> string_prefix_hash(std::string const &str) {
    std::vector<u64> result;

    result.reserve(str.size() + 1);

    u64 prefix_sum = 0;

    result.push_back(prefix_sum);

    for (size_t prefix_len = 1; prefix_len <= str.size(); prefix_len++) {
        prefix_sum = (prefix_sum * 128 + (u64)str[prefix_len - 1]) % MOD;
        result.push_back(prefix_sum);
    }

    return result;
}

// 整个字符串的hash
u64 string_hash(std::string const &str) {
    u64 prefix_sum = 0;

    for (size_t prefix_len = 1; prefix_len <= str.size(); prefix_len++) {
        prefix_sum = (prefix_sum * 128 + (u64)str[prefix_len - 1]) % MOD;
    }

    return prefix_sum;
}
```

## Sort

### 快速排序

时间复杂度：
最坏情况：$O(n^2)$ / 平均情况：$O(n \log n)$ / 最好情况：$O(n \log n)$

```cpp
// 过不了极端数据
// 对于极端数据，请使用三路快速排序

#include <cstdio>
#include <random>
#include <vector>

typedef long long i64;

i64 partition(std::vector<i64> &list, i64 p, i64 r);
void do_quick_sort(std::vector<i64> &list, i64 p, i64 r);
void quick_sort(std::vector<i64> &list);
void random_permutation(std::vector<i64> &list);

int main(void) {
    size_t n = 0;
    std::scanf("%zu", &n);

    std::vector<i64> num_list;

    num_list.reserve(n);

    for (size_t i = 0; i < n; i++) {
        i64 num = 0;
        std::scanf("%lld", &num);
        num_list.push_back(num);
    }

    quick_sort(num_list);

    for (i64 num : num_list) {
        std::printf("%lld ", num);
    }

    std::putchar('\n');

    return 0;
}

// partition range [p, r]
i64 partition(std::vector<i64> &list, i64 p, i64 r) {
    i64 pivot = list[(size_t)r];

    i64 i = p - 1;

    for (i64 j = p; j <= r - 1; j++) {
        if (list[(size_t)j] <= pivot) {
            i++;

            std::swap(list[(size_t)i], list[(size_t)j]);
        }
    }

    std::swap(list[(size_t)i + 1], list[(size_t)r]);

    return i + 1;
}

void do_quick_sort(std::vector<i64> &list, i64 p, i64 r) {
    if (p < r) {
        i64 q = partition(list, p, r);

        do_quick_sort(list, p, q - 1);
        do_quick_sort(list, q + 1, r);
    }
}

void quick_sort(std::vector<i64> &list) {
    if (list.empty()) {
        return;
    }
    random_permutation(list);
    do_quick_sort(list, 0, (i64)(list.size() - 1));
}

void random_permutation(std::vector<i64> &list) {
    auto const seed = 84841984;

    std::mt19937 urbg{seed};

    for (size_t i = 0; i < list.size(); i++) {
        std::uniform_int_distribution<size_t> distr{i, list.size() - 1};

        std::swap(list[i], list[distr(urbg)]);
    }
}
```

### 堆排序

时间复杂度：
最坏情况：$O(n \log n)$ / 平均情况：$O(n \log n)$ / 最好情况：$O(n \log n)$

空间复杂度：$O(n)$ + $O(1)$（辅助空间）

Verdict: https://www.luogu.com.cn/record/195212889

```cpp
#include <cstdio>
#include <utility>
#include <vector>

typedef long long i64;

class Heap {
  public:
    std::vector<i64> heap;
    size_t heap_size;

    explicit Heap() { heap_size = 0; }

    explicit Heap(std::vector<i64> const &num_list) {
        heap = num_list;
        heap_size = num_list.size();

        if (num_list.size() >= 2) {
            size_t idx = num_list.size() / 2 - 1;
            for (size_t i = 0; i <= num_list.size() / 2 - 1; i++) {
                max_heapify(idx);

                idx--;
            }
        }
    }

  public:
    i64 heap_maximum() { return heap[0]; }
    i64 heap_extract_max() {
        i64 max_value = heap[0];

        heap[0] = heap[heap_size - 1];

        max_heapify(0);

        heap_size--;
        return max_value;
    }

    void heap_increase_key(size_t i, i64 new_key) {
        heap[i] = new_key;

        while (i >= 1 && heap[parent(i)] < heap[i]) {
            std::swap(heap[parent(i)], heap[i]);

            i = parent(i);
        }
    }

    void heap_insert(i64 key) {
        heap.push_back(0);

        heap_increase_key(heap_size - 1, key);
        heap_size++;
    }

    void max_heapify(size_t i) {
        while (true) {
            size_t left_idx = left(i);
            size_t right_idx = right(i);

            size_t largest_idx = i;

            if (left_idx < heap_size && heap[left_idx] > heap[i]) {
                largest_idx = left_idx;
            }

            if (right_idx < heap_size && heap[right_idx] > heap[largest_idx]) {
                largest_idx = right_idx;
            }

            if (largest_idx != i) {
                std::swap(heap[i], heap[largest_idx]);

                i = largest_idx;
            } else {
                break;
            }
        }
    }

  private:
    inline size_t left(size_t i) { return 2 * i + 1; }

    inline size_t right(size_t i) { return 2 * i + 2; }

    inline size_t parent(size_t i) { return (i - 1) / 2; }
};

std::vector<i64> heap_sort(std::vector<i64> const &list);

int main(void) {
    size_t n = 0;
    std::scanf("%zu", &n);

    std::vector<i64> num_list;

    num_list.reserve(n);

    for (size_t i = 0; i < n; i++) {
        i64 num = 0;
        std::scanf("%lld", &num);
        num_list.push_back(num);
    }

    std::vector<i64> sorted = heap_sort(num_list);

    for (i64 num : sorted) {
        std::printf("%lld ", num);
    }

    std::putchar('\n');

    return 0;
}

std::vector<i64> heap_sort(std::vector<i64> const &list) {
    Heap heap{list};

    if (list.empty()) {
        return list;
    }

    for (size_t i = 0; i < list.size() - 1; i++) {
        std::swap(heap.heap[0], heap.heap[list.size() - 1 - i]);

        heap.heap_size--;

        heap.max_heapify(0);
    }

    return heap.heap;
}
```

### 三路快速排序

三路快速排序是快速排序的一个变种，用于解决重复元素较多的情况。

时间复杂度：
最坏情况：$O(n^2)$ / 平均情况：$O(n \log n)$ / 最好情况：$O(n)$ （所有元素相同）

```cpp
// From oi wiki

// 模板的 T 参数表示元素的类型，此类型需要定义小于（<）运算
template <typename T>
// arr 为需要被排序的数组，len 为数组长度
void quick_sort(T arr[], const int len) {
    if (len <= 1)
        return;
    // 随机选择基准（pivot）
    const T pivot = arr[rand() % len];
    // i：当前操作的元素下标
    // arr[0, j)：存储小于 pivot 的元素
    // arr[k, len)：存储大于 pivot 的元素
    int i = 0, j = 0, k = len;
    // 完成一趟三路快排，将序列分为：
    // 小于 pivot 的元素 | 等于 pivot 的元素 | 大于 pivot 的元素
    while (i < k) {
        if (arr[i] < pivot)
            swap(arr[i++], arr[j++]);
        else if (pivot < arr[i])
            swap(arr[i], arr[--k]);
        else
            i++;
    }
    // 递归完成对于两个子序列的快速排序
    quick_sort(arr, j);
    quick_sort(arr + k, len - k);
}
```

### 归并排序

时间复杂度：
最坏情况：$O(n \log n)$ / 平均情况：$O(n \log n)$ / 最好情况：$O(n \log n)$

空间复杂度：$O(n)$

```cpp
#include <algorithm>
#include <cstdio>
#include <vector>

typedef long long i64;

void merge(std::vector<i64> &arr, size_t p, size_t q, size_t r);
void do_merge_sort(std::vector<i64> &arr, size_t p, size_t r);
void merge_sort(std::vector<i64> &arr);

int main(void) {
    size_t n = 0;
    std::scanf("%zu", &n);

    std::vector<i64> num_list;

    num_list.reserve(n);

    for (size_t i = 0; i < n; i++) {
        i64 num = 0;
        std::scanf("%lld", &num);
        num_list.push_back(num);
    }

    merge_sort(num_list);

    for (i64 num : num_list) {
        std::printf("%lld ", num);
    }

    std::putchar('\n');

    return 0;
}

// merge sorted [p, q] and [q + 1, r]
void merge(std::vector<i64> &arr, size_t p, size_t q, size_t r) {
    std::vector<i64> aux;
    aux.reserve(r - p + 1);

    size_t i = p;
    size_t j = q + 1;

    while ((i <= q) && (j <= r)) {
        if (arr[i] <= arr[j]) {
            aux.push_back(arr[i]);
            i++;
        } else {
            aux.push_back(arr[j]);
            j++;
        }
    }

    while (i <= q) {
        aux.push_back(arr[i]);
        i++;
    }

    while (j <= r) {
        aux.push_back(arr[j]);
        j++;
    }

    std::copy(std::begin(aux), std::end(aux), std::begin(arr) + (long)p);
}

void do_merge_sort(std::vector<i64> &arr, size_t p, size_t r) {
    if (p == r) {
        return;
    }

    size_t q = p + (r - p) / 2;

    do_merge_sort(arr, p, q);
    do_merge_sort(arr, q + 1, r);

    merge(arr, p, q, r);
}

void merge_sort(std::vector<i64> &arr) {
    if (arr.empty() || arr.size() == 1) {
        return;
    }

    do_merge_sort(arr, 0, arr.size() - 1);
}
```

### 寻找第n大元素

通过快速排序的 partition 操作，可以在O(n)的时间复杂度内找到第n大的元素。

```cpp
#include <vector>

typedef long long i64;

// partition range [p, r]
i64 partition(std::vector<i64> &list, i64 p, i64 r) {
    i64 pivot = list[(size_t)r];

    i64 i = p - 1;

    for (i64 j = p; j <= r - 1; j++) {
        if (list[(size_t)j] <= pivot) {
            i++;

            std::swap(list[(size_t)i], list[(size_t)j]);
        }
    }

    std::swap(list[(size_t)i + 1], list[(size_t)r]);

    return i + 1;
}

// 将改变list！
// nth从0开始

i64 do_nth_element(std::vector<i64> &list, i64 p, i64 r, i64 nth) {
    i64 pivot_idx = partition(list, p, r);

    if (pivot_idx == nth) {
        return list[(size_t)pivot_idx];
    } else if (pivot_idx > nth) {
        return do_nth_element(list, p, pivot_idx - 1, nth);
    } else {
        return do_nth_element(list, pivot_idx + 1, r, nth);
    }
}
```

## Graph/ShortestPath

### Dijkstra 算法

Dijkstra 算法用于计算图中的单源最短路径。其只能处理权值非负的图，且无法检测负环。

Dijkstra 算法的基本思想是贪心算法。它维护一个数组 distance，其中 distance[u]表示从起点到顶点 u 的最短路径的长度。在每一步中，Dijkstra算法选择一个未标记的顶点 u，使得 distance[u] 最小，并标记顶点u。然后，对于所有与顶点 u 相邻的顶点 v，更新 distance[v] = min(distance[v],distance[u] + weight(u, v))。

时间复杂度：$O(M \log{M})$

Verdict：
- P3371: https://www.luogu.com.cn/record/194010420
- P4779: https://www.luogu.com.cn/record/194008908

```cpp
#include <cstddef>
#include <queue>
#include <vector>

#define MY_INFINITY (1LL << 60)

typedef long long i64;

struct Edge {
    size_t to;
    i64 weight;

    explicit constexpr Edge(size_t to_, i64 weight_)
        : to{to_}, weight{weight_} {}
};

struct NodeInfo {
    size_t node;
    i64 distance;

    friend bool operator<(NodeInfo const &left, NodeInfo const &right) {
        return left.distance > right.distance;
    }

    explicit constexpr NodeInfo(size_t node_, i64 distance_)
        : node{node_}, distance{distance_} {}
};

std::vector<i64> dijkstra(std::vector<std::vector<Edge>> const &node_to_edges,
                          size_t source_node) {
    std::vector<i64> result;
    std::vector<int> visited;

    result.resize(node_to_edges.size(), MY_INFINITY);
    visited.resize(node_to_edges.size(), 0);

    std::priority_queue<NodeInfo> node_queue;

    result[source_node] = 0;
    node_queue.emplace(source_node, 0);

    while (!node_queue.empty()) {
        NodeInfo current_node_info = node_queue.top();

        node_queue.pop();

        if (visited[current_node_info.node] == 1) {
            continue;
        }
        for (Edge const &edge : node_to_edges[current_node_info.node]) {
            size_t other_node = edge.to;

            if (result[other_node] >
                result[current_node_info.node] + edge.weight) {
                result[other_node] =
                    result[current_node_info.node] + edge.weight;
                node_queue.emplace(other_node, result[other_node]);
            }
        }

        visited[current_node_info.node] = 1;
    }

    return result;
}
```

### Bellman–Ford 算法

Bellman–Ford 算法用于计算图中的单源最短路径。其可以处理权值为负的图，且可以检测负环。

需要注意的是，以 [S] 点为源点跑 Bellman–Ford 算法时，如果没有给出存在负环的结果，只能说明从 [S] 点出发不能抵达一个负环，而不能说明图上不存在负环。

因此如果需要判断整个图上是否存在负环，最严谨的做法是建立一个超级源点，向图上每个节点连一条权值为 0 的边，然后以超级源点为起点执行 Bellman–Ford 算法。

时间复杂度：$O(MN)$

Verdict:
- C4 G
- https://www.luogu.com.cn/record/194504643

```cpp
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
```

### DAG.cpp

```cpp
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
```

## Graph/MaxFlow

### Dinic 算法

Dinic 算法是求解网络流问题的一种算法，是 Edmonds-Karp 算法的优化版本。

时间复杂度：$O(V^2 * E)$。

Dinic 算法的基本思想是通过多次广度优先搜索（BFS）找到层次图，然后通过多次深度优先搜索（DFS）找到增广路径，最终求解出最大流。

Verdict:
- E4 A
- P3376: https://www.luogu.com.cn/record/196009163

```cpp
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
```

### Edmonds-Karp 算法

处理了反平行边、重边、自环。其中：反平行边新建节点，重边容量相加，自环直接移除。

时间复杂度：$O(V(E^2))$

Verdict:
- E4 A
- P3376: https://www.luogu.com.cn/record/194584006

```cpp
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
```

## Graph/MST

### Kruskal 算法

Kruskal 算法，用于计算最小生成树。

时间复杂度：$O(E \log E + E \alpha(V))$。

Verdict: P3366: https://www.luogu.com.cn/record/189763566

```cpp
#include <algorithm>
#include <cstdio>
#include <numeric>
#include <vector>

struct DisjointSet {
    std::vector<size_t> set;
    std::vector<size_t> set_size;

    explicit DisjointSet(size_t size) {
        set.resize(size);
        set_size.resize(size, 1);
        std::iota(std::begin(set), std::end(set), 0);
    }

    size_t find(size_t x) {
        std::vector<size_t> path(16);

        while (set[x] != x) {
            path.push_back(x);
            x = set[x];
        };

        for (size_t node : path) {
            set[node] = x;
        }

        return x;
    }

    void unite(size_t x, size_t y) {
        size_t x_root = find(x);
        size_t y_root = find(y);

        if (x_root == y_root) {
            return;
        }

        if (set_size[x_root] < set_size[y_root]) {
            set[x_root] = y_root;
            set_size[y_root] += set_size[x_root];
        } else {
            set[y_root] = x_root;
            set_size[x_root] += set_size[y_root];
        }
    }
};

// 1 下标

struct Edge {
    size_t from;
    size_t to;
    int weight;

    explicit constexpr Edge(size_t from_, size_t to_, int weight_)
        : from{from_}, to{to_}, weight{weight_} {};
};

std::vector<Edge> kruskal(std::vector<Edge> &edge_list, size_t node_count);

int main(void) {
    size_t node_count, edge_count;
    node_count = edge_count = 0;

    std::scanf("%zu%zu", &node_count, &edge_count);

    std::vector<Edge> edge_list;

    edge_list.reserve(edge_count);

    for (size_t i = 0; i < edge_count; i++) {
        size_t from, to;
        from = to = 0;
        int weight = 0;

        std::scanf("%zu%zu%d", &from, &to, &weight);

        edge_list.emplace_back(from, to, weight);
    }

    std::vector<Edge> mst_edge_list = kruskal(edge_list, node_count);

    if (mst_edge_list.size() < (node_count - 1)) {
        std::printf("orz\n");
    } else {
        int sum = std::accumulate(
            std::begin(mst_edge_list), std::end(mst_edge_list), 0,
            [](int left, Edge const &right) { return left + right.weight; });
        std::printf("%d\n", sum);
    }

    return 0;
}

std::vector<Edge> kruskal(std::vector<Edge> &edge_list, size_t node_count) {
    std::sort(std::begin(edge_list), std::end(edge_list),
              [](Edge const &a, Edge const &b) { return a.weight < b.weight; });

    DisjointSet set{node_count + 1};

    std::vector<Edge> result;

    for (Edge const &edge : edge_list) {
        size_t from = edge.from;
        size_t to = edge.to;

        size_t from_root = set.find(from);
        size_t to_root = set.find(to);

        if (from_root != to_root) {
            result.push_back(edge);
            set.unite(from_root, to_root);
        }

        if (result.size() == (node_count - 1)) {
            break;
        }
    }

    return result;
}
```

### Prim 算法

Prim 算法，用于计算最小生成树。

时间复杂度：$O((V + E) \log V)$。

Verdict: P3366: https://www.luogu.com.cn/record/195050793

```cpp
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
```

## Graph/Tree

### 最近公共祖先

求树上两节点的最近公共祖先，单次询问复杂度$O(\log{N})$，初始化$O(N\log{N})$

传入的应当是邻接表表示的无向图

Verdict: P3379: https://www.luogu.com.cn/record/195462195

```cpp
#include <array>
#include <cstdio>
#include <utility>
#include <vector>

class LCA {
  public:
    std::vector<std::array<size_t, 32>> parent_info;
    std::vector<size_t> node_depth;

  private:
    std::vector<std::vector<size_t>> const &tree;
    size_t const tree_root;

  public:
    explicit LCA(std::vector<std::vector<size_t>> const &tree, size_t root)
        : tree{tree}, tree_root{root} {
        parent_info.resize(tree.size());
        node_depth.resize(tree.size());

        dfs(tree_root, 0);
    }

  public:
    size_t lca(size_t u, size_t v) {
        if (node_depth[u] > node_depth[v]) {
            std::swap(u, v);
        }

        size_t depth_delta = node_depth[v] - node_depth[u];

        size_t i = 0;

        while (depth_delta > 0) {
            size_t digit = depth_delta & 1;
            depth_delta >>= 1;

            if (digit == 1) {
                v = parent_info[v][i];
            }

            i++;
        }
        while (u != v) {
            for (i = 0; i < 32; i++) {
                size_t idx = 31 - i;

                if (parent_info[u][idx] != parent_info[v][idx]) {
                    u = parent_info[u][idx];
                    v = parent_info[v][idx];

                    break;
                }
            }
        }

        return u;
    }

  private:
    void dfs(size_t root, size_t depth) {
        node_depth[root] = depth;

        for (size_t child_node : tree[root]) {
            parent_info[child_node][0] = root;

            for (size_t i = 1; i < 32; i++) {
                size_t p = parent_info[child_node][i - 1];

                // 2 ^ i = 2 ^ (i - 1) + 2 ^ (i - 1)
                parent_info[child_node][i] = parent_info[p][i - 1];
            }

            dfs(child_node, depth + 1);
        }
    }
};
```

## Graph/StronglyConnectedComponents

### DFS 强连通分量

Verdict: https://www.luogu.com.cn/record/194487781

```cpp
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
```

## Graph/TopoSort

### DFS 拓扑排序

```cpp
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
```

### 拓扑排序

Verdict: P1347: https://www.luogu.com.cn/record/192509279

```cpp
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
```

## FFT

### 大整数乘法

使用FFT实现的大整数乘法。
包含进位、去除最高位的0（见main函数）。

时间复杂度：$O(N\log{N})$

Verdict：
- C6 D
- P1919: https://www.luogu.com.cn/record/191419431

```cpp
#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstring>
#include <vector>

#define BUFFER_LEN 1000005
#define EPS 1e-6

typedef double number;
typedef long long i64;

double const PI = std::acos(-1);

char buffer[BUFFER_LEN] = {0};

using Complex = std::complex<number>; // STL complex
void change(Complex y[], int len);
void fft(Complex y[], int len, int on);
std::vector<number> get_big_int();

class Multiplier {
  private:
    std::vector<Complex> a_coff_list;
    std::vector<Complex> b_coff_list;
    size_t a_len;
    size_t b_len;
    size_t input_len;

  public:
    explicit Multiplier(std::vector<number> const &a_list,
                        std::vector<number> const &b_list) {
        input_len = std::max(a_list.size(), b_list.size());
        size_t len = next_power_of_two(2 * input_len);

        a_len = a_list.size();
        b_len = b_list.size();

        a_coff_list.resize(len);
        b_coff_list.resize(len);

        for (size_t i = 0; i < a_list.size(); i++) {
            a_coff_list[i].real(a_list[i]);
        }

        for (size_t i = 0; i < b_list.size(); i++) {
            b_coff_list[i].real(b_list[i]);
        }
    }

    // 对于一个Multiplier对象，该函数仅允许调用一次！
    std::vector<number> multiply() {
        fft(a_coff_list, false);
        fft(b_coff_list, false);

        std::vector<Complex> c_coff_list;
        c_coff_list.resize(a_coff_list.size());

        std::transform(std::begin(a_coff_list), std::end(a_coff_list),
                       std::begin(b_coff_list), std::begin(c_coff_list),
                       [](Complex a, Complex b) { return a * b; });

        fft(c_coff_list, true);

        std::vector<number> result;
        result.resize(a_len + b_len - 1);

        std::transform(std::begin(c_coff_list),
                       std::begin(c_coff_list) + (long)(a_len + b_len - 1),
                       std::begin(result), [](Complex a) { return a.real(); });

        return result;
    }

  private:
    static size_t next_power_of_two(size_t input) {
        if (input == 0) {
            return 1;
        }

        input--;

        input |= (input >> 1);
        input |= (input >> 2);
        input |= (input >> 4);
        input |= (input >> 8);
        input |= (input >> 16);
        input |= (input >> 32);

        return (input + 1);
    }

    // 同样需要保证 len 是 2 的幂
    // 记 rev[i] 为 i 翻转后的值
    static void change(std::vector<Complex> &y) {
        std::vector<size_t> rev;
        size_t len = y.size();
        rev.resize(len, 0);

        for (size_t i = 0; i < len; ++i) {
            rev[i] = rev[i >> 1] >> 1;
            if (i % 2 != 0) { // 如果最后一位是 1，则翻转成 len/2
                rev[i] |= len >> 1;
            }
        }
        for (size_t i = 0; i < len; ++i) {
            if (i < rev[i]) { // 保证每对数只翻转一次
                std::swap(y[i], y[rev[i]]);
            }
        }
    }

    /*
     * 做 FFT
     * len 必须是 2^k 形式
     * reverse == false 时是 DFT，reverse == true 时是 IDFT
     */
    static void fft(std::vector<Complex> &y, bool reverse) {
        size_t len = y.size();

        // 位逆序置换

        change(y);

        int on = 1;
        if (reverse) {
            on = -1;
        }

        // 模拟合并过程，一开始，从长度为一合并到长度为二，一直合并到长度为
        // len。
        for (size_t h = 2; h <= len; h <<= 1) {
            // wn：当前单位复根的间隔：w^1_h
            Complex wn(cos(2 * PI / (double)h), sin(on * 2 * PI / (double)h));
            // 合并，共 len / h 次。
            for (size_t j = 0; j < len; j += h) {
                // 计算当前单位复根，一开始是 1 = w^0_n，之后是以 wn
                // 为间隔递增： w^1_n
                // ...
                Complex w(1, 0);
                for (size_t k = j; k < j + h / 2; k++) {
                    // 左侧部分和右侧是子问题的解
                    Complex u = y[k];
                    Complex t = w * y[k + h / 2];
                    // 这就是把两部分分治的结果加起来
                    y[k] = u + t;
                    y[k + h / 2] = u - t;
                    // 后半个 「step」 中的ω一定和 「前半个」 中的成相反数
                    // 「红圈」上的点转一整圈「转回来」，转半圈正好转成相反数
                    // 一个数相反数的平方与这个数自身的平方相等
                    w = w * wn;
                }
            }
        }
        // 如果是 IDFT，它的逆矩阵的每一个元素不只是原元素取倒数，还要除以长度
        // len。
        if (on == -1) {
            for (size_t i = 0; i < len; i++) {
                y[i].real(y[i].real() / (double)len);
            }
        }
    }
};

int main(void) {
    std::vector<number> a_digit_list = get_big_int();
    std::vector<number> b_digit_list = get_big_int();

    Multiplier mul{a_digit_list, b_digit_list};

    std::vector<number> temp = mul.multiply();

    std::vector<int> result;
    result.resize(2 * temp.size(), 0);

    int carry = 0;

    // 进位
    for (size_t i = 0; i < temp.size(); i++) {
        int current_digit = (int)std::lround(temp[i]);

        current_digit += carry;

        result[i] = current_digit % 10;
        carry = current_digit / 10;
    }

    size_t result_len = 0;

    // 进位
    for (size_t i = temp.size(); i < result.size(); i++) {
        int current_digit = carry;

        result[i] = current_digit % 10;
        carry = current_digit / 10;

        if (carry == 0 && result[i] == 0) {
            result_len = i;
            break;
        }
    }

    for (size_t i = 0; i < result_len; i++) {
        std::putchar(result[result_len - i - 1] + '0');
    }

    std::putchar('\n');

    return 0;
}

std::vector<number> get_big_int() {
    std::vector<number> result;
    std::scanf("%s", buffer);

    size_t num_len = std::strlen(buffer);

    result.resize(num_len, 0);

    for (size_t i = 0; i < num_len; i++) {
        result[num_len - i - 1] = (number)(buffer[i] - '0');
    }

    return result;
}
```

### 多项式乘法

使用FFT实现的多项式乘法。
注意：不是整数乘法，不包含进位、去除最高位的0。

时间复杂度：$O(N\log{N})$

Verdict：P3803： https://www.luogu.com.cn/record/189563795

```cpp
#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdio>
#include <vector>

#define EPS 1e-6

typedef double number;
typedef long long i64;

double const PI = std::acos(-1);

using Complex = std::complex<number>; // STL complex
void change(Complex y[], int len);
void fft(Complex y[], int len, int on);

class Multiplier {
  private:
    std::vector<Complex> a_coff_list;
    std::vector<Complex> b_coff_list;
    size_t a_len;
    size_t b_len;
    size_t input_len;

  public:
    explicit Multiplier(std::vector<number> const &a_list,
                        std::vector<number> const &b_list) {
        input_len = std::max(a_list.size(), b_list.size());
        size_t len = next_power_of_two(2 * input_len);

        a_len = a_list.size();
        b_len = b_list.size();

        a_coff_list.resize(len);
        b_coff_list.resize(len);

        for (size_t i = 0; i < a_list.size(); i++) {
            a_coff_list[i].real(a_list[i]);
        }

        for (size_t i = 0; i < b_list.size(); i++) {
            b_coff_list[i].real(b_list[i]);
        }
    }

    // 对于一个Multiplier对象，该函数仅允许调用一次！
    std::vector<number> multiply() {
        fft(a_coff_list, false);
        fft(b_coff_list, false);

        std::vector<Complex> c_coff_list;
        c_coff_list.resize(a_coff_list.size());

        std::transform(std::begin(a_coff_list), std::end(a_coff_list),
                       std::begin(b_coff_list), std::begin(c_coff_list),
                       [](Complex a, Complex b) { return a * b; });

        fft(c_coff_list, true);

        std::vector<number> result;
        result.resize(a_len + b_len - 1);

        std::transform(std::begin(c_coff_list),
                       std::begin(c_coff_list) + (long)(a_len + b_len - 1),
                       std::begin(result), [](Complex a) { return a.real(); });

        return result;
    }

  private:
    static size_t next_power_of_two(size_t input) {
        if (input == 0) {
            return 1;
        }

        input--;

        input |= (input >> 1);
        input |= (input >> 2);
        input |= (input >> 4);
        input |= (input >> 8);
        input |= (input >> 16);
        input |= (input >> 32);

        return (input + 1);
    }

    // 同样需要保证 len 是 2 的幂
    // 记 rev[i] 为 i 翻转后的值
    static void change(std::vector<Complex> &y) {
        std::vector<size_t> rev;
        size_t len = y.size();
        rev.resize(len, 0);

        for (size_t i = 0; i < len; ++i) {
            rev[i] = rev[i >> 1] >> 1;
            if (i & 1) { // 如果最后一位是 1，则翻转成 len/2
                rev[i] |= len >> 1;
            }
        }
        for (size_t i = 0; i < len; ++i) {
            if (i < rev[i]) { // 保证每对数只翻转一次
                std::swap(y[i], y[rev[i]]);
            }
        }
    }

    /*
     * 做 FFT
     * len 必须是 2^k 形式
     * reverse == false 时是 DFT，reverse == true 时是 IDFT
     */
    void fft(std::vector<Complex> &y, bool reverse) {
        size_t len = y.size();

        // 位逆序置换

        change(y);

        int on = 1;
        if (reverse) {
            on = -1;
        }

        // 模拟合并过程，一开始，从长度为一合并到长度为二，一直合并到长度为
        // len。
        for (size_t h = 2; h <= len; h <<= 1) {
            // wn：当前单位复根的间隔：w^1_h
            Complex wn(cos(2 * PI / (double)h), sin(on * 2 * PI / (double)h));
            // 合并，共 len / h 次。
            for (size_t j = 0; j < len; j += h) {
                // 计算当前单位复根，一开始是 1 = w^0_n，之后是以 wn
                // 为间隔递增： w^1_n
                // ...
                Complex w(1, 0);
                for (size_t k = j; k < j + h / 2; k++) {
                    // 左侧部分和右侧是子问题的解
                    Complex u = y[k];
                    Complex t = w * y[k + h / 2];
                    // 这就是把两部分分治的结果加起来
                    y[k] = u + t;
                    y[k + h / 2] = u - t;
                    // 后半个 「step」 中的ω一定和 「前半个」 中的成相反数
                    // 「红圈」上的点转一整圈「转回来」，转半圈正好转成相反数
                    // 一个数相反数的平方与这个数自身的平方相等
                    w = w * wn;
                }
            }
        }
        // 如果是 IDFT，它的逆矩阵的每一个元素不只是原元素取倒数，还要除以长度
        // len。
        if (on == -1) {
            for (size_t i = 0; i < len; i++) {
                y[i].real(y[i].real() / (double)len);
                y[i].imag(y[i].imag() / (double)len);
            }
        }
    }
};
```

### 快速傅里叶变换

设$f(x) = a_0x^0 + a_1x^1 + \ldots + a_{n - 1}x^{n - 1}$，则FFT可求出$[f(\omega_n^0), f(\omega_n^1), \ldots, f(\omega_n^{n - 1})]$

时间复杂度：$O(N\log{N})$

Verdict：C6 C
Verdict：P3803： https://www.luogu.com.cn/record/189563795

```cpp
#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdio>
#include <vector>

#define EPS 1e-6

typedef double number;
typedef long long i64;

double const PI = std::acos(-1);

using Complex = std::complex<number>;

// 同样需要保证 len 是 2 的幂
// 记 rev[i] 为 i 翻转后的值
void change(std::vector<Complex> &y) {
    std::vector<size_t> rev;
    size_t len = y.size();
    rev.resize(len, 0);

    for (size_t i = 0; i < len; ++i) {
        rev[i] = rev[i >> 1] >> 1;
        if (i % 2 != 0) { // 如果最后一位是 1，则翻转成 len/2
            rev[i] |= len >> 1;
        }
    }
    for (size_t i = 0; i < len; ++i) {
        if (i < rev[i]) { // 保证每对数只翻转一次
            std::swap(y[i], y[rev[i]]);
        }
    }
}

/*
 * 做 FFT
 * len 必须是 2^k 形式
 * reverse == false 时是 DFT，reverse == true 时是 IDFT
 */
void fft(std::vector<Complex> &y, bool reverse) {
    size_t len = y.size();

    // 位逆序置换

    change(y);

    int on = 1;
    if (reverse) {
        on = -1;
    }

    // 模拟合并过程，一开始，从长度为一合并到长度为二，一直合并到长度为
    // len。
    for (size_t h = 2; h <= len; h <<= 1) {
        // wn：当前单位复根的间隔：w^1_h
        Complex wn(cos(2 * PI / (double)h), sin(on * 2 * PI / (double)h));
        // 合并，共 len / h 次。
        for (size_t j = 0; j < len; j += h) {
            // 计算当前单位复根，一开始是 1 = w^0_n，之后是以 wn
            // 为间隔递增： w^1_n
            // ...
            Complex w(1, 0);
            for (size_t k = j; k < j + h / 2; k++) {
                // 左侧部分和右侧是子问题的解
                Complex u = y[k];
                Complex t = w * y[k + h / 2];
                // 这就是把两部分分治的结果加起来
                y[k] = u + t;
                y[k + h / 2] = u - t;
                // 后半个 「step」 中的ω一定和 「前半个」 中的成相反数
                // 「红圈」上的点转一整圈「转回来」，转半圈正好转成相反数
                // 一个数相反数的平方与这个数自身的平方相等
                w = w * wn;
            }
        }
    }
    // 如果是 IDFT，它的逆矩阵的每一个元素不只是原元素取倒数，还要除以长度
    // len。
    if (on == -1) {
        for (size_t i = 0; i < len; i++) {
            y[i].real(y[i].real() / (double)len);
            y[i].imag(y[i].imag() / (double)len);
        }
    }
}
```

## NumberTheory

### 高精度

注意：只处理了正数

注意：此处的高精度乘法采用朴素方法

注意：保证所有操作后（+，-，*），得到的len反映了实际的数位长度（即，考虑了进位，并删去了前导零）

Verdict：
- P1601：https://www.luogu.com.cn/record/190643889
- P1303：https://www.luogu.com.cn/record/190648675

```cpp
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

struct BigInt {
    std::vector<int> digits;
    size_t len;

    explicit BigInt(size_t capacity) {
        digits.resize(capacity, 0);
        len = 1;
    };

    explicit BigInt(size_t capacity, long long from) {
        digits.resize(capacity, 0);

        size_t i = 0;

        while (from > 0) {
            digits[i] = (int)(from % 10);
            from /= 10;
            i++;
        }

        len = i;

        if (len == 0) {
            len = 1;
        }
    }

    explicit BigInt(size_t capacity, std::string const &from) {
        size_t valid_len = from.size();

        for (size_t i = 0; i < from.size(); i++) {
            if (!std::isalnum(from[i])) {
                valid_len = i;
                break;
            }
        }

        digits.resize(capacity, 0);

        for (size_t i = 0; i < valid_len; i++) {
            digits[i] = from[from.size() - i - 1] - '0';
        }

        len = valid_len;

        if (len == 0) {
            len = 1;
        }
    }

    friend BigInt operator+(BigInt const &left, BigInt const &right) {
        BigInt result{std::max(left.digits.size(), right.digits.size())};

        size_t new_len = 0;

        if (left.len > right.len) {
            std::copy_n(std::begin(left.digits), left.len,
                        std::begin(result.digits));
            for (size_t i = 0; i < right.len; i++) {
                result.digits[i] += right.digits[i];
            }

            new_len = left.len;
        } else {
            std::copy_n(std::begin(right.digits), right.len,
                        std::begin(result.digits));

            for (size_t i = 0; i < left.len; i++) {
                result.digits[i] += left.digits[i];
            }

            new_len = right.len;
        }

        int carry = 0;
        for (size_t i = 0; i < result.digits.size(); i++) {
            result.digits[i] += carry;
            carry = result.digits[i] / 10;
            result.digits[i] %= 10;

            if (i >= new_len && carry == 0 && result.digits[i] == 0) {
                new_len = i;
                break;
            }
        }

        result.len = new_len;

        if (result.len == 0) {
            result.len = 1;
        }
        return result;
    }

    friend BigInt operator-(BigInt const &left, BigInt const &right) {
        BigInt result{left.digits.capacity()};

        std::copy_n(std::begin(left.digits), left.len,
                    std::begin(result.digits));

        for (size_t i = 0; i < left.len; i++) {
            result.digits[i] -= right.digits[i];
        }

        int borrow = 0;

        for (size_t i = 0; i < left.len; i++) {
            result.digits[i] -= borrow;

            if (result.digits[i] < 0) {
                borrow = -result.digits[i] / 10 + 1;
                result.digits[i] += borrow * 10;
            }
        }

        for (size_t i = 0; i < left.len; i++) {
            size_t idx = (left.len - i - 1);

            if (result.digits[i] != 0) {
                result.len = idx + 1;
                break;
            }
        }

        if (result.len == 0) {
            result.len = 1;
        }

        return result;
    }

    friend BigInt operator*(BigInt const &left, BigInt const &right) {
        BigInt result{std::max(left.digits.size(), right.digits.size())};

        for (size_t i = 0; i < left.len; i++) {
            for (size_t j = 0; j < right.len; j++) {
                result.digits[i + j] += left.digits[i] * right.digits[j];
            }
        }

        int carry = 0;
        for (size_t i = 0; i < result.digits.size(); i++) {
            result.digits[i] += carry;
            carry = result.digits[i] / 10;
            result.digits[i] %= 10;
        }

        result.len = 1;

        for (size_t i = 0; i < (left.len + right.len + 1); i++) {
            size_t idx = left.len + right.len - i;

            if (result.digits[idx] != 0) {
                result.len = idx + 1;
                break;
            }
        }

        return result;
    }

    std::string to_string() {
        std::string s;

        s.resize(len, '0');

        for (size_t i = 0; i < len; i++) {
            s[len - i - 1] = (char)digits[i] + '0';
        }

        return s;
    }
};

int main(void) {
    long long a, b;
    a = b = 0;

    std::scanf("%lld%lld", &a, &b);

    BigInt b_a{1000, a};
    BigInt b_b{1000, b};

    std::printf("a + b: %s\n", (b_a + b_b).to_string().data());
    std::printf("a - b: %s\n", (b_a - b_b).to_string().data());
    std::printf("a * b: %s\n", (b_a * b_b).to_string().data());

    return 0;
}
```

### 最大公约数（拓展欧几里得算法）、线性同余方程、中国剩余定理

Verdict：
- B4025：https://www.luogu.com.cn/record/190564296
- P1082：https://www.luogu.com.cn/record/190565417
- P1495：https://www.luogu.com.cn/record/190793944

注：P1495的一个WA由i64溢出导致，相信出题不会这么阴间

```cpp
#include <cstdlib>
#include <numeric>
#include <tuple>
#include <utility>
#include <vector>

typedef long long number;

number gcd(number a, number b) {
    a = std::abs(a);
    b = std::abs(b);
    if (a < b) {
        std::swap(a, b);
    }

    while (b > 0) {
        number temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

// d = gcd(a, b) = ax + by
struct EEResult {
    number d;
    number x;
    number y;
};

EEResult exgcd(number a, number b) {
    a = std::abs(a);
    b = std::abs(b);

    number x = 1, y = 0;

    number x1 = 0, y1 = 1, a1 = a, b1 = b;
    while (b1 > 0) {
        number q = a1 / b1;
        std::tie(x, x1) = std::make_tuple(x1, x - q * x1);
        std::tie(y, y1) = std::make_tuple(y1, y - q * y1);
        std::tie(a1, b1) = std::make_tuple(b1, a1 - q * b1);
    }
    return EEResult{a1, x, y};
}

// Solve ax = b (mod n) for x = x_0
// x_i = x_0 + i * (n / d), d = gcd(a, n)
// solvable if and only if: d | b

struct MLEResult {
    bool solvable;
    number x0;
};

MLEResult modular_linear_equation_solver(number a, number b, number n) {
    EEResult r = exgcd(a, n);

    number d = r.d;

    if (b % d == 0) {
        return MLEResult{true, ((r.x * b / d) + n) % n};
    }
    return MLEResult{false, 0};
}

// n = n_1n_2...n_k
// a = (a_1c_1 + a_2c_2 + ... + a_nc_n) mod n
struct CRTResult {
    number a;
    number n;
};

// n_1, n_2, ..., n_k are pairwise relative prime
// 求同余方程组：x = a_i (mod n_i)的解：x = a (mod n)
CRTResult crt(std::vector<number> const &a_list,
              std::vector<number> const &n_list) {
    number n = std::accumulate(std::begin(n_list), std::end(n_list), 1LL,
                               std::multiplies<number>{});

    number result = 0;
    for (size_t i = 0; i < n_list.size(); i++) {
        number n_i = n_list[i];
        number m_i = n / n_i;

        MLEResult r = modular_linear_equation_solver(m_i, 1, n_i);

        number c_i = (r.x0 % n * m_i % n) % n;

        result = (result + (a_list[i] * c_i) % n) % n;
    }

    return CRTResult{result, n};
}
```

### 快速求模n下的a ^ b

时间复杂度：$O(\log{b})$

```cpp
typedef long long number;

// a ^ b mod n
number modular_exponentiation(number a, number b, number n) {
    number result = 1;
    number temp = a % n;

    while (b > 0) {
        number current_digit = b % 2;

        if (current_digit == 1) {
            result = (result * temp) % n;
        }

        temp = (temp * temp) % n;

        b /= 2;
    }

    return result;
}
```

## Hash

### SipHash 哈希算法密钥生成

由输入的字符串生成密钥（只考虑前128位），虽然方便，但随机性可能没有随机生成的好

```c
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char *s_gets(char *dst, int n);

int main(void) {
    char buffer[255] = {0};
    s_gets(buffer, 255);

    uint64_t k_0 = 0;
    uint64_t k_1 = 0;

    memcpy(&k_0, buffer, 8);
    memcpy(&k_1, buffer + 8, 8);

    printf("k_0 = 0x%016lx\n", k_0);
    printf("k_1 = 0x%016lx\n", k_1);
}

char *s_gets(char *dst, int n) {
    char *ret = fgets(dst, n, stdin);
    int i = 0;
    if (ret != NULL) {
        while (dst[i] != '\n' && dst[i] != '\0') {
            i++;
        }
        if (dst[i] == '\n') {
            dst[i] = '\0';
        } else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
        }
    }

    return ret;
}
```

### SipHash 哈希算法（用于Rust标准库中的相关容器）

该算法需要输入一个128位的密钥（由k_0，k_1两个u64组成），对任意长度的消息生成64位的摘要值。

```c
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define SIPHASH_COMPRESSION_ROUND 2
#define SIPHASH_FINALIZATION_ROUND 4

#define ROTL(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))

uint64_t siphash(uint64_t k_0, uint64_t k_1, void *msg, size_t msg_len);

uint64_t siphash(uint64_t k_0, uint64_t k_1, void *msg, size_t msg_len) {
    uint64_t v_0 = k_0 ^ 0x736f6d6570736575;
    uint64_t v_1 = k_1 ^ 0x646f72616e646f6d;
    uint64_t v_2 = k_0 ^ 0x6c7967656e657261;
    uint64_t v_3 = k_1 ^ 0x7465646279746573;

    size_t normal_count = msg_len / 8;

    uint64_t *current_word_ptr = (uint64_t *)msg;

    for (size_t i = 0; i < normal_count; i++) {
        uint64_t current_word = *current_word_ptr;

        v_3 ^= current_word;

        for (int j = 0; j < SIPHASH_COMPRESSION_ROUND; j++) {
            // SipRound
            {
                v_0 += v_1;
                v_2 += v_3;

                v_1 = ROTL(v_1, 13);
                v_3 = ROTL(v_3, 16);

                v_1 ^= v_0;
                v_3 ^= v_2;

                v_0 = ROTL(v_0, 32);

                v_2 += v_1;
                v_0 += v_3;

                v_1 = ROTL(v_1, 17);
                v_3 = ROTL(v_3, 21);

                v_1 ^= v_2;
                v_3 ^= v_0;

                v_2 = ROTL(v_2, 32);
            }
        }

        v_0 ^= current_word;

        current_word_ptr++;
    }

    uint64_t final_word = 0;

    memcpy(&final_word, current_word_ptr, msg_len - normal_count * 8);

    final_word |= (msg_len % 256) << 56;

    v_3 ^= final_word;

    for (int j = 0; j < SIPHASH_COMPRESSION_ROUND; j++) {
        // SipRound
        {
            v_0 += v_1;
            v_2 += v_3;

            v_1 = ROTL(v_1, 13);
            v_3 = ROTL(v_3, 16);

            v_1 ^= v_0;
            v_3 ^= v_2;

            v_0 = ROTL(v_0, 32);

            v_2 += v_1;
            v_0 += v_3;

            v_1 = ROTL(v_1, 17);
            v_3 = ROTL(v_3, 21);

            v_1 ^= v_2;
            v_3 ^= v_0;

            v_2 = ROTL(v_2, 32);
        }
    }

    v_0 ^= final_word;

    v_2 ^= 0xff;

    for (int j = 0; j < SIPHASH_FINALIZATION_ROUND; j++) {
        // SipRound
        {
            v_0 += v_1;
            v_2 += v_3;

            v_1 = ROTL(v_1, 13);
            v_3 = ROTL(v_3, 16);

            v_1 ^= v_0;
            v_3 ^= v_2;

            v_0 = ROTL(v_0, 32);

            v_2 += v_1;
            v_0 += v_3;

            v_1 = ROTL(v_1, 17);
            v_3 = ROTL(v_3, 21);

            v_1 ^= v_2;
            v_3 ^= v_0;

            v_2 = ROTL(v_2, 32);
        }
    }

    return v_0 ^ v_1 ^ v_2 ^ v_3;
}

int main(void) {

    uint64_t k_0 = 0x0706050403020100;
    uint64_t k_1 = 0x0f0e0d0c0b0a0908;

    uint8_t buf[15] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                       0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e};

    uint64_t result = siphash(k_0, k_1, buf, 15);
    uint64_t expected = 0xa129ca6149be45e5;

    return 0;
}
```

## ComputationalGeometry

### 计算几何

包含内容：

- 点（相等与Hash，极坐标角排序）
- 向量（加、减、点积*、叉积^）
- 线段（是否相交）
- 凸包
- 多边形周长
- 多边形面积（两倍）

```cpp
#include <algorithm>
#include <cmath>
#include <vector>

#define MY_INFINITY (1LL << 60)

typedef long long number;

class Point {
  public:
    number x;
    number y;

    explicit constexpr Point(number a, number b) : x{a}, y{b} {};

    friend Point operator-(Point const &left, Point const &right) {
        return Point{left.x - right.x, left.y - right.y};
    }

    friend bool operator==(Point const &left, Point const &right) {
        return (left.x == right.x) && (left.y == right.y);
    }
};

struct PointHash {
    std::size_t operator()(Point const &k) const noexcept {
        size_t right = (size_t)k.y;

        right += 0x9e3779b97f4a7c15;

        right = ((right << 31) | (right >> (64 - 31)));

        return ((size_t)k.x ^ right);
    }
};

class Vector {
  public:
    number x;
    number y;

    explicit constexpr Vector(number a, number b) : x{a}, y{b} {};
    explicit constexpr Vector(Point const &point) : x{point.x}, y{point.y} {};
    explicit constexpr Vector(Point const &from, Point const &to)
        : x{to.x - from.x}, y{to.y - from.y} {};

    friend Vector operator+(Vector const &left, Vector const &right) {
        return Vector{left.x + right.x, left.y + right.y};
    }

    friend Vector operator-(Vector const &left, Vector const &right) {
        return Vector{left.x - right.x, left.y - right.y};
    }

    friend Vector operator-(Vector const &self) {
        return Vector{-self.x, -self.y};
    }

    friend number operator*(Vector const &left, Vector const &right) {
        return left.x * right.x + left.y * right.y;
    }

    friend number operator^(Vector const &left, Vector const &right) {
        return left.x * right.y - right.x * left.y;
    }

    double module() const {
        return std::sqrt(this->x * this->x + this->y * this->y);
    }
};

class Segment {
  public:
    Point from;
    Point to;

    explicit constexpr Segment(Point a, Point b) : from{a}, to{b} {};

    Vector direction() const { return Vector{to.x - from.x, to.y - from.y}; }

    Segment reversed() const { return Segment{to, from}; }

    // Verdict： C5 D
    bool on_segment(Point p) const {
        Vector d = this->direction();

        Vector d2 = Vector{p - this->from};

        if ((d ^ d2) == 0) {
            if (p.x >= std::min(from.x, to.x) &&
                p.x <= std::max(from.x, to.x) &&
                p.y >= std::min(from.y, to.y) &&
                p.y <= std::max(from.y, to.y)) {
                return true;
            }
        }

        return false;
    }

    // Verdict： C5 D
    bool intersect(Segment const &s) const {
        Vector this_direction = this->direction();
        Vector s_direction = s.direction();

        Vector d1 = Vector{s.from - this->from};
        Vector d2 = Vector{s.from - this->to};
        Vector d3 = Vector{this->from - s.from};
        Vector d4 = Vector{this->from - s.to};

        number pro1 = s_direction ^ d1;
        number pro2 = s_direction ^ d2;
        number pro3 = this_direction ^ d3;
        number pro4 = this_direction ^ d4;

        if (((pro1 > 0 && pro2 < 0) || (pro1 < 0 && pro2 > 0)) &&
            ((pro3 > 0 && pro4 < 0) || (pro3 < 0 && pro4 > 0))) {
            return true;
        }

        if (pro1 == 0 && s.on_segment(this->from)) {
            return true;
        }

        if (pro2 == 0 && s.on_segment(this->to)) {
            return true;
        }

        if (pro3 == 0 && on_segment(s.from)) {
            return true;
        }

        if (pro4 == 0 && on_segment(s.to)) {
            return true;
        }

        return false;
    }
};

// 按极角排序
// Verdict：C5 F
// Verdict: P2742：https://www.luogu.com.cn/record/189107703
bool operator<(Point const &left, Point const &right) {
    number cross_product = Vector{left} ^ Vector { right };
    if (cross_product > 0) {
        return true;
    }
    if (cross_product == 0) {
        if (left.x > right.x) {
            return false;
        }
        if (left.x < right.x) {
            return true;
        }
        return left.y <= right.y;
    }
    return false;
}

// 无需对point_list进行预处理
// 要求：不得含重复点！
// Verdict：C5 F
// Verdict: P2742：https://www.luogu.com.cn/record/189107703
std::vector<Point> convex_hull(std::vector<Point> point_list) {
    number min_y = MY_INFINITY;
    number min_x = MY_INFINITY;
    auto origin_element = std::begin(point_list);

    for (Point const &point : point_list) {
        if (point.y < min_y) {
            min_y = point.y;
        }
    }

    for (auto iter = std::begin(point_list); iter != std::end(point_list);
         iter++) {
        if (iter->y == min_y) {
            if (iter->x < min_x) {
                min_x = iter->x;
                origin_element = iter;
            }
        }
    }

    for (Point &point : point_list) {
        point.x -= min_x;
        point.y -= min_y;
    }

    point_list.erase(origin_element);

    std::sort(std::begin(point_list), std::end(point_list));
    std::vector<Point> stack;

    stack.reserve(point_list.size());

    stack.emplace_back(0, 0);
    stack.push_back(point_list[0]);
    stack.push_back(point_list[1]);

    for (size_t i = 2; i < point_list.size(); i++) {
        while (true) {
            Point const &current_top = stack.back();
            Point const &current_next_to_top = stack[stack.size() - 2];
            Vector v1 = Vector{current_next_to_top, current_top};
            Vector v2 = Vector{current_next_to_top, point_list[i]};

            if ((v1 ^ v2) > 0) {
                break;
            }

            stack.pop_back();

            if (stack.size() <= 1) {
                break;
            }
        }

        stack.push_back(point_list[i]);
    }

    return stack;
}

// 求周长（若为线段，则为2*线段长度）
// Verdict: P2742：https://www.luogu.com.cn/record/189107703
double length(std::vector<Point> const &convex_hull_list) {
    double result = 0.0;

    size_t len = convex_hull_list.size();
    for (size_t i = 0; i < len; i++) {
        Point const &p1 = convex_hull_list[i];
        Point const &p2 = convex_hull_list[(i + 1) % len];

        Vector v{p1, p2};

        result += v.module();
    }

    return result;
}

// 求多边形面积（返回2 * 面积）
// 注意转换为double除以2可能有精度问题！
// Verdict：C5 F
number doubled_polygon_area(std::vector<Point> const &sorted_convex_list) {
    Point const &aux_point = *std::begin(sorted_convex_list);

    number doubled_area = 0;

    size_t len = sorted_convex_list.size();

    for (size_t i = 0; i < len; i++) {
        Vector v1{aux_point, sorted_convex_list[i]};
        Vector v2{aux_point, sorted_convex_list[(i + 1) % len]};

        doubled_area += (v1 ^ v2);
    }

    return std::abs(doubled_area);
}
```

### 找凸包

```c
#include <stdio.h>
#include <stdlib.h>

// 定义 点 结构体
typedef struct Point {
    long long x, y;
} point;

// 初始化点集，栈
point p[100005], stack[100005];
int top = -1;
point min = (point){0x7fffffffLL, 0x7fffffffLL};

// 叉积
long long cross(point s, point a, point b) {
    return (a.x - s.x) * (b.y - s.y) - (a.y - s.y) * (b.x - s.x);
}

// 两点间距离的平方
long long dist(point a, point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

// 比较函数 - 按极角排序
int cmp(const void *a, const void *b) {
    const point *p1 = (const point *)a;
    const point *p2 = (const point *)b;
    long long c = cross(min, *p1, *p2);
    if (c > 0) {
        return -1;
    }
    if (c < 0) {
        return 1;
    }
    return dist(*p1, min) > dist(*p2, min) ? 1 : -1;
}

// 找凸包 - stack中剩余元素即为凸包点集
void convex_hull(point *p, int n, point *stack) {
    top = -1;
    min = (point){0x7fffffffLL, 0x7fffffffLL};
    for (int i = 0; i < n; i++) {
        scanf("%lld%lld", &p[i].x, &p[i].y);
        // 找到最下最左的点
        if (p[i].y < min.y || (p[i].y == min.y && p[i].x < min.x)) {
            min = p[i];
        }
    }
    // 按极角排序
    qsort(p, (size_t)n, sizeof(point), cmp);
    // 栈初始化
    stack[++top] = p[0];
    stack[++top] = p[1];
    for (int i = 2; i < n; i++) {
        // 右转则出栈
        while (top > 0 && cross(stack[top - 1], stack[top], p[i]) <= 0) {
            top--;
        }
        // 入栈
        stack[++top] = p[i];
    }
    // 处理最后一个点
    if (top > 1) {
        while (top > 0 && cross(stack[top - 1], stack[top], p[0]) <= 0) {
            top--;
        }
    }
}
```

## DynamicProgramming

### 最长不增子序列

最长不增子序列，动态规划+二分，O(NlogN)。
若要求最长不减子序列，将数组中所有元素取负即可；
若要求最长递减子序列（即，不能出现相等的情况），在调用二分查找时将val + 1即可。
注意最长不增子序列的长度为dp中的最大值。

Verdict:
- B3637: https://www.luogu.com.cn/record/195490309
- P1439: https://www.luogu.com.cn/record/195524254
- E3.H

```cpp
#include <vector>

#define MY_INFINITY (1LL << 60)

typedef long long i64;
i64 get_length(std::vector<i64> const &list, i64 left, i64 right, i64 val);
std::vector<i64> calc_dp(std::vector<i64> const &num_list);

std::vector<i64> calc_dp(std::vector<i64> const &num_list) {
    std::vector<i64> dp;
    dp.resize(num_list.size(), 0);

    std::vector<i64> max_last_element_by_length;

    max_last_element_by_length.resize(num_list.size() + 1, -MY_INFINITY);

    max_last_element_by_length[0] = MY_INFINITY;

    dp[0] = 1;
    max_last_element_by_length[1] = num_list[0];

    for (size_t i = 1; i < num_list.size(); i++) {
        i64 prev_max_length =
            get_length(max_last_element_by_length, 1, (i64)i, num_list[i]);

        dp[i] = prev_max_length + 1;

        if (max_last_element_by_length[(size_t)dp[i]] < num_list[i]) {
            max_last_element_by_length[(size_t)dp[i]] = num_list[i];
        }
    }

    return dp;
}

i64 get_length(std::vector<i64> const &list, i64 left, i64 right, i64 val) {
    while (left <= right) {
        i64 mid = (left + right) / 2;

        if (list[(size_t)mid] < val) {
            right = mid - 1;
        } else if (list[(size_t)mid] > val) {
            left = mid + 1;
        } else {
            left = mid + 1;
        }
    }

    return right;
}
```

## Other

### heap.cpp

```cpp
#include <utility>
#include <vector>

typedef long long i64;

// Verdict: https://www.luogu.com.cn/record/195208664
// Verdict: https://www.luogu.com.cn/record/195212889

class Heap {
  public:
    std::vector<i64> heap;
    size_t heap_size;

    explicit Heap() { heap_size = 0; }

    explicit Heap(std::vector<i64> const &num_list) {
        heap = num_list;
        heap_size = num_list.size();

        if (num_list.size() >= 2) {
            size_t idx = num_list.size() / 2 - 1;
            for (size_t i = 0; i <= num_list.size() / 2 - 1; i++) {
                max_heapify(idx);

                idx--;
            }
        }
    }

  public:
    i64 heap_maximum() { return heap[0]; }
    i64 heap_extract_max() {
        i64 max_value = heap[0];

        heap[0] = heap[heap_size - 1];

        max_heapify(0);

        heap_size--;
        return max_value;
    }

    void heap_increase_key(size_t i, i64 new_key) {
        heap[i] = new_key;

        while (i >= 1 && heap[parent(i)] < heap[i]) {
            std::swap(heap[parent(i)], heap[i]);

            i = parent(i);
        }
    }

    void heap_insert(i64 key) {
        heap.push_back(0);

        heap_increase_key(heap_size - 1, key);
        heap_size++;
    }

  private:
    inline size_t left(size_t i) { return 2 * i + 1; }

    inline size_t right(size_t i) { return 2 * i + 2; }

    inline size_t parent(size_t i) { return (i - 1) / 2; }

    void max_heapify(size_t i) {
        while (1) {
            size_t left_idx = left(i);
            size_t right_idx = right(i);

            size_t largest_idx = i;

            if (left_idx < heap_size && heap[left_idx] > heap[i]) {
                largest_idx = left_idx;
            }

            if (right_idx < heap_size && heap[right_idx] > heap[largest_idx]) {
                largest_idx = right_idx;
            }

            if (largest_idx != i) {
                std::swap(heap[i], heap[largest_idx]);

                i = largest_idx;
            } else {
                break;
            }
        }
    }
};
```

### monotone_queue.cpp

```cpp
#include <cstdio>
#include <deque>
#include <vector>

class MonotoneQueue {
  private:
    struct QueueItem {
        int data;
        size_t idx;

        explicit constexpr QueueItem(int data_, size_t idx_)
            : data{data_}, idx{idx_} {};
    };

    std::deque<QueueItem> queue;
    size_t window_length;
    bool increasing;

    size_t idx;

  public:
    explicit MonotoneQueue(size_t window_length_, bool increasing_)
        : window_length{window_length_}, increasing{increasing_}, idx{0} {}

    void put(int data) {
        idx++;

        while (!queue.empty() && (idx - queue.front().idx) >= window_length) {
            queue.pop_front();
        }

        if (increasing) {
            while (!queue.empty() && queue.back().data > data) {
                queue.pop_back();
            }
        } else {
            while (!queue.empty() && queue.back().data < data) {
                queue.pop_back();
            }
        }

        queue.emplace_back(data, idx);
    }

    int get() const { return queue.front().data; }
};

int main(void) {
    size_t n, k;
    n = k = 0;

    std::scanf("%zu%zu", &n, &k);

    std::vector<int> input_list;
    input_list.reserve(n);

    for (size_t i = 0; i < n; i++) {
        int data = 0;
        std::scanf("%d", &data);
        input_list.push_back(data);
    }

    MonotoneQueue min_queue{k, true};
    MonotoneQueue max_queue{k, false};

    for (size_t i = 0; i < n; i++) {
        min_queue.put(input_list[i]);

        if (i >= (k - 1)) {
            std::printf("%d ", min_queue.get());
        }
    }

    std::putchar('\n');

    for (size_t i = 0; i < n; i++) {
        max_queue.put(input_list[i]);

        if (i >= (k - 1)) {
            std::printf("%d ", max_queue.get());
        }
    }

    std::putchar('\n');

    return 0;
}
```

## AdvancedDataStructures

### 并查集

并查集是一种用于管理元素所属集合的数据结构，实现为一个森林，其中每棵树表示一个集合，树中的节点表示对应集合中的元素。若大小为$size$，可用节点编号为$0$到$size - 1$（零下标）

每个操作的平均时间几乎为$O(1)$

Verdict: P3367: https://www.luogu.com.cn/record/189760008

```cpp
#include <cstddef>
#include <numeric>
#include <vector>

using std::size_t;

struct DisjointSet {
    // 节点i的父亲
    std::vector<size_t> set;

    // 以节点i为树根的子树的大小
    std::vector<size_t> set_size;

    explicit DisjointSet(size_t size) {
        set.resize(size);
        set_size.resize(size, 1);
        std::iota(std::begin(set), std::end(set), 0);
    }

    // 查询对应元素的根节点（+路径压缩）
    size_t find(size_t x) {
        std::vector<size_t> path(16);

        while (set[x] != x) {
            path.push_back(x);
            x = set[x];
        };

        for (size_t node : path) {
            set[node] = x;
        }

        return x;
    }

    // 合并两个元素所属的集合（+启发式合并）
    void unite(size_t x, size_t y) {
        size_t x_root = find(x);
        size_t y_root = find(y);

        if (x_root == y_root) {
            return;
        }

        if (set_size[x_root] < set_size[y_root]) {
            set[x_root] = y_root;
            set_size[y_root] += set_size[x_root];
        } else {
            set[y_root] = x_root;
            set_size[x_root] += set_size[y_root];
        }
    }
};
```

