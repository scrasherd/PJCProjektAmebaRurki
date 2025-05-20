#include "MapRenderer.h"
#include "FoodGradientMapRenderer.h"
#include "VectorMapRenderer.h"
#include "PressureMapRenderer.h"


std::unique_ptr<MapRenderer> MapRenderer::create(RenderMode mode) {
    switch (mode) {
    case RenderMode::Food:
        return std::make_unique<FoodGradientMapRenderer>();
    case RenderMode::Vector:
        return std::make_unique<VectorMapRenderer>();
    case RenderMode::Pressure:
        return std::make_unique<PressureMapRenderer>();
    default:
        throw std::runtime_error("Nieobs³ugiwany tryb renderowania");
    }
}
