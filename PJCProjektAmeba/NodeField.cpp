#define _USE_MATH_DEFINES
#include "NodeField.h"
#include "Tube.h"
#include "Vec2.h"
#include "Vec2i.h"
#include "LineInteresction.h"
#include <cmath>
#include <algorithm>
#include <iostream>


NodeField::NodeField(int width, int height, float cellSize)
    : Grid(width, height), cellSize(cellSize)
{}

float NodeField::getCellSize() const {
    return cellSize;
}

Node* NodeField::getNodeFromGridPosition(const Vec2i& gridPos) const {
    static constexpr int blockSize = GridBlock<Node*>::getBlockSize();
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    auto it = NodeGrid.find({ blockX, blockY });
    if (it == NodeGrid.end()) return nullptr;

    return it->second.get(localX, localY);
}

Node*& NodeField::getCellRef(const Vec2i& gridPos) {
    static constexpr int blockSize = GridBlock<Node*>::getBlockSize();
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    return NodeGrid[{blockX, blockY}].ref(localX, localY);
}

void NodeField::addNodeToGrid(const Vec2& pos, Node* node) {
    Vec2i gridPos = toGridCoords(pos);
    getCellRef(gridPos) = node;
}


const std::unordered_map<std::pair<int, int>, GridBlock<Node* >, PairHash>& NodeField::getNodeGrid() const {
    return NodeGrid;
}

void NodeField::clearGrid() {
    NodeGrid.clear();
}


Node* NodeField::findNodeToConnect(const Vec2& posA, const Vec2& posB, float angle, float tubeLength) const {
    const Vec2i posAGrid = toGridCoords(posA);
    const Vec2i posBGrid = toGridCoords(posB);

    int aX = posAGrid.getXi();
    int aY = posAGrid.getYi();
    int bX = posBGrid.getXi();
    int bY = posBGrid.getYi();

    int cX = static_cast<int>(std::round((aX + bX) / 2.0f));
    int cY = static_cast<int>(std::round((aY + bY) / 2.0f));

    std::vector<Vec2i> directions = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} }; //Prawo, dó³, lewo, góra

    int bbLengthHalf = static_cast<int>(std::ceil((tubeLength * 1.1f * (std::abs(std::cos(angle)) + std::abs(std::sin(angle)))) / 2.0f));

    int cells = bbLengthHalf * bbLengthHalf;
    int stepsNeeded= 1;
    int stepsMade = 0;
    int currentDir = 0;
    int turns = 0;

    int X = cX;
    int Y = cY;

    for (int i = 0; i < cells; i++) {

        X = X + directions[currentDir].getXi();
        Y = Y + directions[currentDir].getYi();

        Node* node = getNodeFromGridPosition(Vec2i(X, Y));
        if (node && CheckForIntersection(posAGrid, posBGrid, node)) {
            return node;
        }

        stepsMade++;
        if (stepsMade == stepsNeeded) {
            stepsMade = 0;
            currentDir = (currentDir + 1) % 4;
            turns++;

            if (turns == 2) {
                stepsNeeded++;
                turns = 0;
            }
        }


    }

        return nullptr;
}

const bool NodeField::CheckForIntersection(const Vec2i& posA, const Vec2i& posB, const Node* node) const {

    Vec2i posC = toGridCoords(node->getPosition());

    for (Tube* tube : node->getConnectedTubes()) {
        const Node* neighbor = tube->getOtherNode(node);
        Vec2i posD = toGridCoords(neighbor->getPosition());
        
        if (checkIfLinesIntersect(posA, posB, posC, posD)) {
            return true;
        }

    }
    return false;
}