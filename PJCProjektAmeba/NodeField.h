#pragma once
#include "Vec2.h"
#include "Vec2i.h"
#include "Grid.h"
#include "Node.h"
#include <vector>
#include <utility>
#include <unordered_map>


class NodeField : public Grid {
private:


    float cellSize;
    std::unordered_map<std::pair<int, int>, GridBlock<Node* >, PairHash> NodeGrid;

public:
    NodeField (Plasmodium& plasmodium, int width, int height, float cellSize);

    float getCellSize() const override;

    //virtual float& getCellRef(const Vec2i& gridPos) override;
    //virtual float getCellVal(const Vec2i& gridPos) const override;

    //virtual void mark(const Vec2& pos, float value) override;

    const Node* getNodeFromGridPosition(const Vec2i& gridPos) const;
    Node*& getCellRef(const Vec2i& gridPos);

    void addNodeToGrid(const Vec2& pos, Node* node);

    const Node* findNodeToConnect(const Vec2& posA, const Vec2& posB, float angle, float tubeLength);
    bool CheckForIntersection(const Vec2i& posA, const Vec2i& posB, const Node* node);

    const std::unordered_map<std::pair<int, int>, GridBlock<Node* >, PairHash>& getNodeGrid() const;

    virtual void clearGrid() override;

};

