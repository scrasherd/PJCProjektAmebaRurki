#pragma once
#include "IMapRenderer.h"
#include <unordered_map>
#include <functional>
#include <memory>

class MapRendererRegistry {
public:
    using RendererFactory = std::function<std::unique_ptr<IMapRenderer>()>;

    static void registerRenderer(RenderMode mode, RendererFactory factory);
    static std::unique_ptr<IMapRenderer> create(RenderMode mode);

private:
    static std::unordered_map<RenderMode, RendererFactory>& getFactories();
};

