#include "world/world.hpp"

#include <raylib.h>

#include "player/player.hpp"
#include "world/worldData.hpp"

World::World()
    : m_scenes{WorldData::CreateScenes()}
{
}

Scene* World::FindScene(const SceneCoord& coord)
{
    for (Scene& scene : m_scenes)
    {
        if (scene.coord == coord)
        {
            return &scene;
        }
    }

    return nullptr;
}

const Scene* World::FindScene(const SceneCoord& coord) const
{
    for (const Scene& scene : m_scenes)
    {
        if (scene.coord == coord)
        {
            return &scene;
        }
    }

    return nullptr;
}

bool World::HasSceneAt(const SceneCoord& coord) const
{
    return FindScene(coord) != nullptr;
}

std::optional<Direction> World::CheckTransitionCollision(
    const Player& player,
    const SceneCoord& currentCoord) const
{
    const Scene* currentScene = FindScene(currentCoord);
    if (currentScene == nullptr || !currentScene->allowEdgeTransitions)
    {
        return std::nullopt;
    }

    const SceneCoord left{currentCoord.x - 1, currentCoord.y};
    const SceneCoord right{currentCoord.x + 1, currentCoord.y};
    const SceneCoord up{currentCoord.x, currentCoord.y - 1};
    const SceneCoord down{currentCoord.x, currentCoord.y + 1};

    const Scene* leftScene = FindScene(left);
    const Scene* rightScene = FindScene(right);
    const Scene* upScene = FindScene(up);
    const Scene* downScene = FindScene(down);

    if (leftScene != nullptr && leftScene->allowEdgeTransitions &&
        CheckCollisionRecs(player.rect, MakeLeftTransition()))
    {
        return Direction::Left;
    }

    if (rightScene != nullptr && rightScene->allowEdgeTransitions &&
        CheckCollisionRecs(player.rect, MakeRightTransition()))
    {
        return Direction::Right;
    }

    if (upScene != nullptr && upScene->allowEdgeTransitions &&
        CheckCollisionRecs(player.rect, MakeUpTransition()))
    {
        return Direction::Up;
    }

    if (downScene != nullptr && downScene->allowEdgeTransitions &&
        CheckCollisionRecs(player.rect, MakeDownTransition()))
    {
        return Direction::Down;
    }

    return std::nullopt;
}

SceneCoord World::GetNextCoord(const SceneCoord& currentCoord, Direction direction) const
{
    switch (direction)
    {
        case Direction::Left:
            return SceneCoord{currentCoord.x - 1, currentCoord.y};
        case Direction::Right:
            return SceneCoord{currentCoord.x + 1, currentCoord.y};
        case Direction::Up:
            return SceneCoord{currentCoord.x, currentCoord.y - 1};
        case Direction::Down:
            return SceneCoord{currentCoord.x, currentCoord.y + 1};
    }

    return currentCoord;
}

void World::DrawAvailableTransitions(const SceneCoord& currentCoord) const
{
    const Scene* currentScene = FindScene(currentCoord);
    if (currentScene == nullptr || !currentScene->allowEdgeTransitions)
    {
        return;
    }

    const SceneCoord left{currentCoord.x - 1, currentCoord.y};
    const SceneCoord right{currentCoord.x + 1, currentCoord.y};
    const SceneCoord up{currentCoord.x, currentCoord.y - 1};
    const SceneCoord down{currentCoord.x, currentCoord.y + 1};

    const Scene* leftScene = FindScene(left);
    const Scene* rightScene = FindScene(right);
    const Scene* upScene = FindScene(up);
    const Scene* downScene = FindScene(down);

    if (leftScene != nullptr && leftScene->allowEdgeTransitions)
    {
        DrawRectangleRec(MakeLeftTransition(), BLACK);
    }

    if (rightScene != nullptr && rightScene->allowEdgeTransitions)
    {
        DrawRectangleRec(MakeRightTransition(), BLACK);
    }

    if (upScene != nullptr && upScene->allowEdgeTransitions)
    {
        DrawRectangleRec(MakeUpTransition(), BLACK);
    }

    if (downScene != nullptr && downScene->allowEdgeTransitions)
    {
        DrawRectangleRec(MakeDownTransition(), BLACK);
    }
}