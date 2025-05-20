#define _USE_MATH_DEFINES
#include "Plasmodium.h"
#include "Tube.h"
#include "Node.h"
#include "FoodField.h"
#include "math.h"
#include <cmath>
#include <memory>

Plasmodium::Plasmodium(const Vec2& startPosition, float tubeLength, FoodField& foodField)
    : rng(std::random_device{}()), tubeLength(tubeLength), flowModel(*this, foodField) {
    
    addStartStructure(startPosition, tubeLength, 1.0f /*œrednica*/, 1000.0f /*cytoplazma*/);
}

Vec2 randomDirection(std::mt19937& rng) {
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
    float angle = angleDist(rng);
    return Vec2(std::cos(angle), std::sin(angle));
}

void Plasmodium::addStartStructure(const Vec2& centerPos, float radius, float tubeDiameter, float cytValue) {
    // Tworzenie wêz³a centralnego
    auto center = std::make_unique<Node>(centerPos);
    Node* centerPtr = center.get();
    nodes.push_back(std::move(center));

    // Rozmieszczanie 3 wêz³ów na okrêgu wokó³ centrum (k¹ty co 120 stopni)
    for (int i = 0; i < 3; ++i) {
        float angle = i * (2.0f * 3.1415926f / 3.0f); // 0, 120°, 240°
        Vec2 offset{ radius * std::cos(angle), radius * std::sin(angle) };
        Vec2 outerPos = centerPtr->getPosition() + offset;

        auto outer = std::make_unique<Node>(outerPos);
        Node* outerPtr = outer.get();
        nodes.push_back(std::move(outer));

        auto tube = std::make_unique<Tube>(centerPtr, outerPtr, tubeDiameter, cytValue);
        tubes.push_back(std::move(tube));
    }

    // Dodanie centralnego wêz³a jako potencjalnego zakoñczenia
    EndingNodes.push_back(centerPtr);
}

void Plasmodium::growOneStep(const FoodField& foodField) {
    if (EndingNodes.empty()) return;

    sortEndings(foodField);

    int total = static_cast<int>(EndingNodes.size());
    int activeCount = std::max(1, total / 10); // 10% najlepszych, min. 1

    std::uniform_real_distribution<float> skipChance(0.0f, 1.0f);

    for (int i = 0; i < activeCount; ++i) {
        Node* parent = EndingNodes[i];

        // 5% szans na pominiêcie danego node'a
        bool shouldGrow = skipChance(rng) >= 0.05f;

        if (shouldGrow) {
            Vec2 parentPos = parent->getPosition();

            std::discrete_distribution<int> branchDist({ 50, 25, 25 });
            int newBranches = branchDist(rng) + 1;

            int direction = 0;
            if (newBranches > 1) {
                std::uniform_int_distribution<int> DirectionDist(0, 1);
                direction = DirectionDist(rng) == 0 ? 1 : -1;
            }

            float baseAngle = computeAngle(parent);
            int attempts = 0;
            int successful = 0;

            while (successful < newBranches && attempts < 10) {
                float newAngle = generateAngle(baseAngle, successful, direction, foodField, parentPos);
                Vec2 dir(std::cos(newAngle), std::sin(newAngle));
                Vec2 newPos = parentPos + dir * tubeLength;

                // SprawdŸ czy nowy wêze³ nie jest za blisko innych
                bool tooClose = false;
                for (const auto& node : nodes) {
                    if (node->getPosition().distanceTo(newPos) < tubeLength * 0.9f) {
                        tooClose = true;
                        break;
                    }
                }

                if (!tooClose) {
                    auto newNode = std::make_unique<Node>(newPos);
                    auto newTube = std::make_unique<Tube>(parent, newNode.get(), 1.0f, 0.0f);

                    EndingNodes.push_back(newNode.get());

                    auto it = std::find(EndingNodes.begin(), EndingNodes.end(), parent);
                    if (it != EndingNodes.end()) {
                        EndingNodes.erase(it);
                    }

                    nodes.push_back(std::move(newNode));
                    tubes.push_back(std::move(newTube));
                    ++successful;
                }

                ++attempts;
            }
        }
    }
}

void Plasmodium::sortEndings(const FoodField& foodField) {
    std::sort(EndingNodes.begin(), EndingNodes.end(),
        [&](Node* A, Node* B) {
            Vec2 positionA = A->getPosition();
            Vec2 positionB = B->getPosition();

            float valueA = foodField.getValueAt(positionA);
            float valueB = foodField.getValueAt(positionB);

            return valueA > valueB; // malej¹co — najwy¿sze wartoœci na górze
        });
}

float Plasmodium::computeAngle(Node* parent) {
    float baseAngle = 0;

    if (parent->getConnectedTubes().empty()) {
        // Brak rurek – losuj dowolny k¹t
        std::uniform_real_distribution<float> angleDist(0.0f, 2 * M_PI);
        baseAngle = angleDist(rng);
    }else {
        // Kierunek ostatniej rurki
        Tube* tube = parent->getConnectedTubes()[0];
        Node* neighbor = (tube->getNodeA() == parent) ? tube->getNodeB() : tube->getNodeA();
        Vec2 lastDir = (parent->getPosition() - neighbor->getPosition()).normalized();
        baseAngle = std::atan2(lastDir.getY(), lastDir.getX());
    }

    return baseAngle;
}

float Plasmodium::generateAngle(float baseAngle, int generated, int direction, const FoodField& foodField, const Vec2& position) {
    float meanAngle = baseAngle;
    float spread = M_PI / 24;
    float newAngle = 0;

    if (generated == 0) {
        meanAngle = baseAngle;               
        spread = M_PI / 6;                    
    }
    else if (generated == 1) {
        meanAngle = baseAngle + direction * M_PI * 3.f / 8.f; 
        spread = M_PI / 6;                       
    }
    else if (generated == 2) {
        meanAngle = baseAngle - direction * M_PI * 3.f / 8.f; 
        spread = M_PI / 6;
    }

    int x = position.getX();
    int y = position.getY();

    float dx = foodField.getValueAt(Vec2(x + 1, y)) - foodField.getValueAt(Vec2(x - 1, y));
    float dy = foodField.getValueAt(Vec2(x - 1, y)) - foodField.getValueAt(Vec2(x, y - 1));
    Vec2 grad(dx, dy);

    if (grad.length() > 0.001f) {
        grad = grad.normalized();
        float foodAngle = std::atan2(grad.getY(),grad.getX());

        //Przesuñ œrodek rozk³adu trochê w stronê gradientu
        float influence = 0.5f; // 0 = ignoruj jedzenie, 1 = tylko jedzenie
        meanAngle = std::lerp(meanAngle, foodAngle, influence);
    }

    std::normal_distribution<float> angleDist(meanAngle, spread);
    return angleDist(rng);
}

void Plasmodium::simulateFlow(float dt) {
    flowModel.updatePhasesAndPressures(dt);
    flowModel.computeFlow();
}


const std::vector<std::unique_ptr<Node>>& Plasmodium::getNodes() const {
    return nodes;
}

const std::vector<std::unique_ptr<Tube>>& Plasmodium::getTubes() const {
    return tubes;
}
