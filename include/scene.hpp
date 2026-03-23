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
    Decoration,
    Bush,
    Tree,
    HouseEntrance
};

struct SceneObject
{
    SceneObjectType type{};
    Rectangle rect{};
    bool blocksMovement{false};
    bool isInteractable{false};
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

[[nodiscard]] SceneObject MakeWall(float x, float y, float width, float height);
[[nodiscard]] SceneObject MakeRock(float x, float y, float width, float height);
[[nodiscard]] SceneObject MakeLadder(float x, float y, float width, float height);
[[nodiscard]] SceneObject MakeDecoration(float x, float y, float width, float height, Color color);
[[nodiscard]] SceneObject MakeBush(float x, float y, float width, float height);
[[nodiscard]] SceneObject MakeTree(float x, float y, float width, float height);
[[nodiscard]] SceneObject MakeHouseEntrance(float x, float y, float width, float height);

[[nodiscard]] const SceneObject* FindInteractableObject(const Rectangle& playerRect, const Scene& scene);