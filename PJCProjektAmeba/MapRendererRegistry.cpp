#include "MapRendererRegistry.h"

void MapRendererRegistry::registerRenderer(RenderMode mode, RendererFactory factory) {
    getFactories()[mode] = std::move(factory);
}

std::unique_ptr<IMapRenderer> MapRendererRegistry::create(RenderMode mode) {
    auto& factories = getFactories();
    auto it = factories.find(mode);
    if (it != factories.end()) {
        return it->second();
    }
    return nullptr;
}

std::unordered_map<RenderMode, MapRendererRegistry::RendererFactory>& MapRendererRegistry::getFactories() {
    static std::unordered_map<RenderMode, RendererFactory> factories;
    return factories;
}
