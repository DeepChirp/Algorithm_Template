/*
 * name: 找凸包
 * description: 给定平面上的n个点，求这些点的凸包点集
 */

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