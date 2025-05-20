#pragma once
#include <cmath>

class Vec2 {

private:
    float x, y;

public:
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}

    float getX() const{
        return x;
    }

    float getY() const{
        return y;
    }

    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vec2 normalized() const {
        float len = length();
        return (len > 0.0f) ? (*this * (1.0f / len)) : Vec2(0, 0);
    }

    float distanceTo(const Vec2& other) const {
        return (*this - other).length();
    }
};

