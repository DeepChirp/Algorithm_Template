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