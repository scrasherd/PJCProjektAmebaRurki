#pragma once
#include "Vec2.h"
#include "Vec2i.h"
#include <vector>
#include <utility>
#include <unordered_map>

class Plasmodium;

template<typename T>
class GridBlock {
private:
    static constexpr int blockSize = 8;
    T values[blockSize][blockSize]{};

public:
    T get(int x, int y) const { return values[x][y]; }
    void set(int x, int y, T value) { values[x][y] = value; }
    T& ref(int x, int y) { return values[x][y]; }

    static constexpr int getBlockSize() { return blockSize; }
};

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

class Grid {
protected:

    Plasmodium& plasmodium;
    int width, height;


public:
    Grid(Plasmodium& plasmodium, int width, int height);
    virtual ~Grid() = default;

    virtual float getCellSize() const = 0;
    Vec2i toGridCoords(const Vec2& pos) const;

    bool isInside(const Vec2i& gridPos) const;

    //virtual float& getCellRef(const Vec2i& pos) = 0;
    //virtual float getCellVal(const Vec2i& posy) const = 0;

    //virtual void mark(const Vec2& pos, float value) = 0;
;
    int getWidth() const;
    int getHeight() const;

    virtual void clearGrid() = 0;
};
