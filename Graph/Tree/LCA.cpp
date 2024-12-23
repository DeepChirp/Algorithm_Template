// 求树上两节点的最近公共祖先，单次询问复杂度$O(\log{N})$，初始化$O(N\log{N})$
// 传入的应当是邻接表表示的无向图
// Verdict: P3379: https://www.luogu.com.cn/record/195462195

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
