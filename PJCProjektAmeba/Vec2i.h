#pragma once
#pragma once
#include <cmath>

class Vec2i {

private:
    int x, y;

public:
    Vec2i() : x(0), y(0) {}
    Vec2i(int x, int y) : x(x), y(y) {}

    int getXi() const {
        return x;
    }

    int getYi() const {
        return y;
    }

    Vec2i operator+(const Vec2i& other) const {
        return Vec2i(x + other.x, y + other.y);
    }

    Vec2i operator-(const Vec2i& other) const {
        return Vec2i(x - other.x, y - other.y);
    }

    Vec2i operator*(int scalar) const {
        return Vec2i(x * scalar, y * scalar);
    }

    bool operator==(const Vec2i& other) const {
        return x == other.x && y == other.y;
    }

    int cross(const Vec2i& other) const {
        return x * other.y - y * other.x;
    }
};