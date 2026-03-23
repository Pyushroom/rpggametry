#pragma once

#include "player.hpp"
#include "scene.hpp"

#include <optional>
#include <vector>

class World
{
public:
    World();

    [[nodiscard]] const Scene* FindScene(const SceneCoord& coord) const;
    [[nodiscard]] bool HasSceneAt(const SceneCoord& coord) const;

    [[nodiscard]] std::optional<Direction> CheckTransitionCollision(
        const Player& player,
        const SceneCoord& currentCoord) const;

    [[nodiscard]] SceneCoord GetNextCoord(
        const SceneCoord& currentCoord,
        Direction direction) const;

    void DrawAvailableTransitions(const SceneCoord& currentCoord) const;

private:
    struct SceneDefinition
    {
        SceneCoord coord{};
        Color backgroundColor{};
        const char* name{};
        std::vector<SceneObject> objects{};
    };

private:
    std::vector<Scene> m_scenes;

    void CreateScenes();
    void AddScene(const SceneDefinition& definition);
};