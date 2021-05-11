#include "scene/Scene.hpp"

auto main() -> int {
    constexpr auto WIDTH = 800, HEIGHT = 600;

    auto app = engine::Scene(WIDTH, HEIGHT, "cpp-engine");
    app.start();

    return 0;
}
