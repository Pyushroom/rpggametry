#pragma once

#include <raylib.h>

#include <optional>
#include <vector>

enum class Direction
{
    Left,
    Right,
    Up,
    Down
};

struct SceneCoord
{
    int x{};
    int y{};

    [[nodiscard]] bool operator==(const SceneCoord& other) const;
};

struct Scene
{
    SceneCoord coord{};
    Color backgroundColor{};
    const char* name{};
    std::vector<Rectangle> walls{};
};

[[nodiscard]] Rectangle MakeLeftTransition();
[[nodiscard]] Rectangle MakeRightTransition();
[[nodiscard]] Rectangle MakeUpTransition();
[[nodiscard]] Rectangle MakeDownTransition();

[[nodiscard]] bool CollidesWithWalls(const Rectangle& rect, const Scene& scene);

void DrawWalls(const Scene& scene);
void DrawSceneInfo(const Scene& scene);