#include "world.hpp"

World::World()
{
    CreateScenes();
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
    const SceneCoord left{currentCoord.x - 1, currentCoord.y};
    const SceneCoord right{currentCoord.x + 1, currentCoord.y};
    const SceneCoord up{currentCoord.x, currentCoord.y - 1};
    const SceneCoord down{currentCoord.x, currentCoord.y + 1};

    if (HasSceneAt(left) && CheckCollisionRecs(player.rect, MakeLeftTransition()))
    {
        return Direction::Left;
    }

    if (HasSceneAt(right) && CheckCollisionRecs(player.rect, MakeRightTransition()))
    {
        return Direction::Right;
    }

    if (HasSceneAt(up) && CheckCollisionRecs(player.rect, MakeUpTransition()))
    {
        return Direction::Up;
    }

    if (HasSceneAt(down) && CheckCollisionRecs(player.rect, MakeDownTransition()))
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
    if (HasSceneAt(SceneCoord{currentCoord.x - 1, currentCoord.y}))
    {
        DrawRectangleRec(MakeLeftTransition(), BLACK);
    }

    if (HasSceneAt(SceneCoord{currentCoord.x + 1, currentCoord.y}))
    {
        DrawRectangleRec(MakeRightTransition(), BLACK);
    }

    if (HasSceneAt(SceneCoord{currentCoord.x, currentCoord.y - 1}))
    {
        DrawRectangleRec(MakeUpTransition(), BLACK);
    }

    if (HasSceneAt(SceneCoord{currentCoord.x, currentCoord.y + 1}))
    {
        DrawRectangleRec(MakeDownTransition(), BLACK);
    }
}

void World::CreateScenes()
{
    AddScene(SceneDefinition{
        SceneCoord{0, 0},
        GREEN,
        "Zielona Polana",
        {
            Rectangle{180.0f, 140.0f, 140.0f, 40.0f},
            Rectangle{500.0f, 220.0f, 220.0f, 40.0f},
            Rectangle{320.0f, 420.0f, 280.0f, 40.0f}
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{-1, 0},
        BLUE,
        "Blekitne Pole",
        {
            Rectangle{240.0f, 120.0f, 40.0f, 260.0f},
            Rectangle{520.0f, 260.0f, 180.0f, 40.0f}
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{1, 0},
        ORANGE,
        "Pomaranczowa Droga",
        {
            Rectangle{180.0f, 300.0f, 520.0f, 40.0f},
            Rectangle{700.0f, 160.0f, 40.0f, 220.0f}
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{0, -1},
        PURPLE,
        "Fioletowe Wzgorze",
        {
            Rectangle{220.0f, 180.0f, 120.0f, 40.0f},
            Rectangle{420.0f, 180.0f, 120.0f, 40.0f},
            Rectangle{620.0f, 180.0f, 120.0f, 40.0f}
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{0, 1},
        LIME,
        "Dolna Laka",
        {
            Rectangle{160.0f, 180.0f, 40.0f, 280.0f},
            Rectangle{360.0f, 120.0f, 40.0f, 280.0f},
            Rectangle{560.0f, 180.0f, 40.0f, 280.0f}
        }
    });
}

void World::AddScene(const SceneDefinition& definition)
{
    Scene scene{};
    scene.coord = definition.coord;
    scene.backgroundColor = definition.backgroundColor;
    scene.name = definition.name;
    scene.walls = definition.walls;

    m_scenes.push_back(scene);
}