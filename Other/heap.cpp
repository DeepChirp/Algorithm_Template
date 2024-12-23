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