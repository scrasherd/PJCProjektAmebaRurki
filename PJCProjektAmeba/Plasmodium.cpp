#include "Plasmodium.h"
#include "Tube.h"
#include "Node.h"
#include <cmath>

Plasmodium::Plasmodium(const Vec2& startPosition, float tubeLength)
    : rng(std::random_device{}()), tubeLength(tubeLength) {
    nodes.push_back(std::make_unique<Node>(startPosition));
}

Vec2 randomDirection(std::mt19937& rng) {
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
    float angle = angleDist(rng);
    return Vec2(std::cos(angle), std::sin(angle));
}

void Plasmodium::growOneStep() {
    std::vector<Node*> endings;
    for (auto& node : nodes) {
        if (node->getConnectedTubes().size() <= 1)
            endings.push_back(node.get());
    }

    if (endings.empty()) return;

    std::uniform_int_distribution<> pick(0, endings.size() - 1);
    Node* parent = endings[pick(rng)];
    Vec2 parentPos = parent->getPosition();

    std::uniform_int_distribution<int> branchCountDist(1, 2);
    int newBranches = branchCountDist(rng);

    int attempts = 0;
    int successful = 0;

    while (successful < newBranches && attempts < 10) {
        Vec2 dir = randomDirection(rng);
        Vec2 newPos = parentPos + dir * tubeLength;

        bool tooClose = false;
        for (const auto& node : nodes) {
            if (node->getPosition().distanceTo(newPos) < tubeLength * 0.9f) {
                tooClose = true;
                break;
            }
        }

        if (!tooClose) {
            auto newNode = std::make_unique<Node>(newPos);
            auto newTube = std::make_unique<Tube>(parent, newNode.get(), 1.0f);

            nodes.push_back(std::move(newNode));
            tubes.push_back(std::move(newTube));
            ++successful;
        }

        ++attempts;
    }
}

const std::vector<std::unique_ptr<Node>>& Plasmodium::getNodes() const {
    return nodes;
}

const std::vector<std::unique_ptr<Tube>>& Plasmodium::getTubes() const {
    return tubes;
}
