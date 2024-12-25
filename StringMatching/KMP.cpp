/*
 * name: KMP，字符串有限状态机
 * description:
 *
 * KMP：
 * - `KMPMatcher(std::string const &pattern)`：创建以pattern为模式的字符串匹配“状态机”
 * - `match(std::string const &to_match)`：返回`to_match`中所有匹配位置的起始下标（0下标）
 * - `void generate_FA_transition_table(size_t delta[MAX_STR_LEN][ALPHABET_LEN])`：利用$\pi$数组生成字符串有限状态机的状态转移表
 *
 *
 * Verdict: P3375: https://www.luogu.com.cn/record/192756655
 * Verdict：E6 A, B, D
 */

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
