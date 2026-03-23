#pragma once

#include <raylib.h>

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

enum class SceneObjectType
{
    Wall,
    Rock,
    Ladder,
    Decoration
};

struct SceneObject
{
    SceneObjectType type{};
    Rectangle rect{};
    bool blocksMovement{false};
    Color color{};
};

struct Scene
{
    SceneCoord coord{};
    Color backgroundColor{};
    const char* name{};
    std::vector<SceneObject> objects{};
};

[[nodiscard]] Rectangle MakeLeftTransition();
[[nodiscard]] Rectangle MakeRightTransition();
[[nodiscard]] Rectangle MakeUpTransition();
[[nodiscard]] Rectangle MakeDownTransition();

[[nodiscard]] bool CollidesWithBlockingObjects(const Rectangle& rect, const Scene& scene);

void DrawSceneObjects(const Scene& scene);
void DrawSceneInfo(const Scene& scene);

// helpers
[[nodiscard]] SceneObject MakeWall(Rectangle rect);
[[nodiscard]] SceneObject MakeRock(Rectangle rect);
[[nodiscard]] SceneObject MakeLadder(Rectangle rect);
[[nodiscard]] SceneObject MakeDecoration(Rectangle rect, Color color);