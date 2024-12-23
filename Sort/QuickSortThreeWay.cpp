/*
 * name: 三路快速排序
 *
 * description:
 * 三路快速排序是快速排序的一个变种，用于解决重复元素较多的情况。
 *
 * 时间复杂度：
 * 最坏情况：$O(n^2)$ / 平均情况：$O(n \log n)$ / 最好情况：$O(n)$ （所有元素相同）
 */

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
