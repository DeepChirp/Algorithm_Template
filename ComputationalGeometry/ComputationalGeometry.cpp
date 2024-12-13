/*
 * name: 计算几何
 * description:
 *
 * 包含内容：

    - 点（相等与Hash，极坐标角排序）
    - 向量（加、减、点积*、叉积^）
    - 线段（是否相交）
    - 凸包
    - 多边形周长
    - 多边形面积（两倍）

 */

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