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
            MakeWall(180.0f, 140.0f, 140.0f, 40.0f),
            MakeRock(500.0f, 220.0f, 70.0f, 70.0f),
            MakeRock(620.0f, 220.0f, 100.0f, 60.0f),
            MakeBush(320.0f, 420.0f, 80.0f, 60.0f),
            MakeBush(430.0f, 420.0f, 80.0f, 60.0f),
            MakeTree(760.0f, 120.0f, 90.0f, 140.0f),
            MakeHouseEntrance(80.0f, 260.0f, 60.0f, 90.0f)
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{-1, 0},
        BLUE,
        "Blekitne Pole",
        {
            MakeWall(240.0f, 120.0f, 40.0f, 260.0f),
            MakeLadder(520.0f, 220.0f, 36.0f, 120.0f),
            MakeRock(620.0f, 260.0f, 80.0f, 60.0f),
            MakeBush(120.0f, 420.0f, 100.0f, 50.0f),
            MakeTree(760.0f, 180.0f, 100.0f, 150.0f)
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{1, 0},
        ORANGE,
        "Pomaranczowa Droga",
        {
            MakeWall(180.0f, 300.0f, 520.0f, 40.0f),
            MakeWall(700.0f, 160.0f, 40.0f, 220.0f),
            MakeDecoration(120.0f, 160.0f, 40.0f, 40.0f, GOLD),
            MakeBush(120.0f, 380.0f, 90.0f, 50.0f),
            MakeHouseEntrance(780.0f, 420.0f, 70.0f, 100.0f)
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{0, -1},
        PURPLE,
        "Fioletowe Wzgorze",
        {
            MakeRock(220.0f, 180.0f, 120.0f, 40.0f),
            MakeRock(420.0f, 180.0f, 120.0f, 40.0f),
            MakeRock(620.0f, 180.0f, 120.0f, 40.0f),
            MakeLadder(450.0f, 340.0f, 36.0f, 120.0f),
            MakeTree(120.0f, 120.0f, 100.0f, 150.0f),
            MakeTree(760.0f, 120.0f, 100.0f, 150.0f)
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{0, 1},
        LIME,
        "Dolna Laka",
        {
            MakeWall(160.0f, 180.0f, 40.0f, 280.0f),
            MakeWall(360.0f, 120.0f, 40.0f, 280.0f),
            MakeWall(560.0f, 180.0f, 40.0f, 280.0f),
            MakeBush(700.0f, 120.0f, 120.0f, 80.0f),
            MakeBush(720.0f, 240.0f, 100.0f, 70.0f),
            MakeHouseEntrance(100.0f, 500.0f, 70.0f, 90.0f)
        }
    });
}

void World::AddScene(const SceneDefinition& definition)
{
    Scene scene{};
    scene.coord = definition.coord;
    scene.backgroundColor = definition.backgroundColor;
    scene.name = definition.name;
    scene.objects = definition.objects;

    m_scenes.push_back(scene);
}