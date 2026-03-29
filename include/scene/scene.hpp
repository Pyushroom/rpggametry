#pragma once

#include <raylib.h>

#include "dialogue/dialogue.hpp"
#include "enemy/enemyData.hpp"
#include "npc/npcData.hpp"

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

enum class SceneObjectType
{
    Wall,
    Rock,
    Ladder,
    Decoration,
    Bush,
    Tree,
    HouseEntrance,
    Npc,
    Enemy
};

enum class InteractionType
{
    None,
    Teleport,
    Dialogue
};

struct SceneObject
{
    SceneObjectType type{};
    Rectangle rect{};
    bool blocksMovement{false};
    bool isInteractable{false};
    Color color{};

    InteractionType interactionType{InteractionType::None};
    const char* promptText{nullptr};

    const DialogueData* dialogueData{nullptr};
    const NpcData* npcData{nullptr};
    const EnemyData* enemyData{nullptr};

    bool hasTargetScene{false};
    SceneCoord targetSceneCoord{};
    Vector2 targetPlayerPosition{};

    bool isDefeated{false};
};

struct Scene
{
    SceneCoord coord{};
    Color backgroundColor{};
    const char* name{};
    bool allowEdgeTransitions{true};
    std::vector<SceneObject> objects{};
};

[[nodiscard]] Rectangle MakeLeftTransition();
[[nodiscard]] Rectangle MakeRightTransition();
[[nodiscard]] Rectangle MakeUpTransition();
[[nodiscard]] Rectangle MakeDownTransition();

[[nodiscard]] bool CollidesWithBlockingObjects(const Rectangle& rect, const Scene& scene);
[[nodiscard]] const SceneObject* FindInteractableObjectNearby(const Rectangle& playerRect, const Scene& scene);
[[nodiscard]] std::optional<std::size_t> FindEnemyCollisionIndex(const Rectangle& playerRect, const Scene& scene);

void DrawSceneObjects(const Scene& scene);
void DrawSceneInfo(const Scene& scene);

[[nodiscard]] SceneObject MakeWall(float x, float y, float width, float height);
[[nodiscard]] SceneObject MakeRock(float x, float y, float width, float height);
[[nodiscard]] SceneObject MakeLadder(
    float x,
    float y,
    float width,
    float height,
    Vector2 targetPlayerPosition,
    const char* promptText);
[[nodiscard]] SceneObject MakeDecoration(float x, float y, float width, float height, Color color);
[[nodiscard]] SceneObject MakeBush(float x, float y, float width, float height);
[[nodiscard]] SceneObject MakeTree(float x, float y, float width, float height);
[[nodiscard]] SceneObject MakeNpc(
    float x,
    float y,
    float width,
    float height,
    const NpcData* npcData);
[[nodiscard]] SceneObject MakeEnemy(
    float x,
    float y,
    float width,
    float height,
    const EnemyData* enemyData);
[[nodiscard]] SceneObject MakeHouseEntrance(
    float x,
    float y,
    float width,
    float height,
    SceneCoord targetSceneCoord,
    Vector2 targetPlayerPosition,
    const char* promptText);