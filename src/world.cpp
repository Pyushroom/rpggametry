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
    const Scene* currentScene = FindScene(currentCoord);
    if (currentScene == nullptr)
    {
        return std::nullopt;
    }

    if (!currentScene->allowEdgeTransitions)
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

    if (leftScene != nullptr &&
        leftScene->allowEdgeTransitions &&
        CheckCollisionRecs(player.rect, MakeLeftTransition()))
    {
        return Direction::Left;
    }

    if (rightScene != nullptr &&
        rightScene->allowEdgeTransitions &&
        CheckCollisionRecs(player.rect, MakeRightTransition()))
    {
        return Direction::Right;
    }

    if (upScene != nullptr &&
        upScene->allowEdgeTransitions &&
        CheckCollisionRecs(player.rect, MakeUpTransition()))
    {
        return Direction::Up;
    }

    if (downScene != nullptr &&
        downScene->allowEdgeTransitions &&
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
    if (currentScene == nullptr)
    {
        return;
    }

    if (!currentScene->allowEdgeTransitions)
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

void World::CreateScenes()
{
    AddScene(SceneDefinition{
        SceneCoord{0, 0},
        GREEN,
        "Zielona Polana",
        true,
        {
            MakeWall(180.0f, 140.0f, 140.0f, 40.0f),
            MakeRock(500.0f, 220.0f, 70.0f, 70.0f),
            MakeRock(620.0f, 220.0f, 100.0f, 60.0f),
            MakeBush(320.0f, 420.0f, 80.0f, 60.0f),
            MakeBush(430.0f, 420.0f, 80.0f, 60.0f),
            MakeTree(760.0f, 120.0f, 90.0f, 140.0f),
            MakeNpc(
                260.0f,
                260.0f,
                40.0f,
                60.0f,
                "Nacisnij E, aby porozmawiac",
                "Witaj podrozniku! To dopiero poczatek twojej przygody."),
            MakeHouseEntrance(
                80.0f,
                260.0f,
                60.0f,
                90.0f,
                SceneCoord{10, 10},
                Vector2{440.0f, 500.0f},
                "Nacisnij E, aby wejsc do domu")
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{-1, 0},
        BLUE,
        "Blekitne Pole",
        true,
        {
            MakeWall(240.0f, 120.0f, 40.0f, 260.0f),
            MakeLadder(
                520.0f,
                220.0f,
                36.0f,
                120.0f,
                Vector2{520.0f, 60.0f},
                "Nacisnij E, aby wejsc po drabinie"),
            MakeLadder(
                520.0f,
                40.0f,
                36.0f,
                120.0f,
                Vector2{520.0f, 260.0f},
                "Nacisnij E, aby zejsc po drabinie"),
            MakeRock(620.0f, 260.0f, 80.0f, 60.0f),
            MakeBush(120.0f, 420.0f, 100.0f, 50.0f),
            MakeTree(760.0f, 180.0f, 100.0f, 150.0f),
            MakeNpc(
                340.0f,
                420.0f,
                40.0f,
                60.0f,
                "Nacisnij E, aby porozmawiac",
                "Ta drabina prowadzi na wyzszy poziom pola.")
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{1, 0},
        ORANGE,
        "Pomaranczowa Droga",
        true,
        {
            MakeWall(180.0f, 300.0f, 520.0f, 40.0f),
            MakeWall(700.0f, 160.0f, 40.0f, 220.0f),
            MakeDecoration(120.0f, 160.0f, 40.0f, 40.0f, GOLD),
            MakeBush(120.0f, 380.0f, 90.0f, 50.0f),
            MakeNpc(
                540.0f,
                120.0f,
                40.0f,
                60.0f,
                "Nacisnij E, aby porozmawiac",
                "Za ta droga kiedys powstanie miasto."),
            MakeHouseEntrance(
                780.0f,
                420.0f,
                70.0f,
                100.0f,
                SceneCoord{11, 10},
                Vector2{440.0f, 500.0f},
                "Nacisnij E, aby wejsc do domu")
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{0, -1},
        PURPLE,
        "Fioletowe Wzgorze",
        true,
        {
            MakeRock(220.0f, 180.0f, 120.0f, 40.0f),
            MakeRock(420.0f, 180.0f, 120.0f, 40.0f),
            MakeRock(620.0f, 180.0f, 120.0f, 40.0f),
            MakeLadder(
                450.0f,
                340.0f,
                36.0f,
                120.0f,
                Vector2{450.0f, 120.0f},
                "Nacisnij E, aby wejsc po drabinie"),
            MakeLadder(
                450.0f,
                80.0f,
                36.0f,
                120.0f,
                Vector2{450.0f, 380.0f},
                "Nacisnij E, aby zejsc po drabinie"),
            MakeTree(120.0f, 120.0f, 100.0f, 150.0f),
            MakeTree(760.0f, 120.0f, 100.0f, 150.0f),
            MakeNpc(
                520.0f,
                420.0f,
                40.0f,
                60.0f,
                "Nacisnij E, aby porozmawiac",
                "Na wzgorzu latwiej wypatrzyc droge przed soba.")
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{0, 1},
        LIME,
        "Dolna Laka",
        true,
        {
            MakeWall(160.0f, 180.0f, 40.0f, 280.0f),
            MakeWall(360.0f, 120.0f, 40.0f, 280.0f),
            MakeWall(560.0f, 180.0f, 40.0f, 280.0f),
            MakeBush(700.0f, 120.0f, 120.0f, 80.0f),
            MakeBush(720.0f, 240.0f, 100.0f, 70.0f),
            MakeNpc(
                260.0f,
                520.0f,
                40.0f,
                60.0f,
                "Nacisnij E, aby porozmawiac",
                "Na lace jest spokojnie. Idealne miejsce na odpoczynek."),
            MakeHouseEntrance(
                100.0f,
                500.0f,
                70.0f,
                90.0f,
                SceneCoord{12, 10},
                Vector2{440.0f, 500.0f},
                "Nacisnij E, aby wejsc do domu")
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{10, 10},
        DARKBROWN,
        "Wnetrze Domu 1",
        false,
        {
            MakeWall(120.0f, 120.0f, 720.0f, 40.0f),
            MakeWall(120.0f, 120.0f, 40.0f, 400.0f),
            MakeWall(800.0f, 120.0f, 40.0f, 400.0f),
            MakeWall(120.0f, 480.0f, 300.0f, 40.0f),
            MakeWall(540.0f, 480.0f, 300.0f, 40.0f),
            MakeDecoration(260.0f, 220.0f, 120.0f, 80.0f, BEIGE),
            MakeDecoration(560.0f, 220.0f, 120.0f, 80.0f, BEIGE),
            MakeNpc(
                420.0f,
                240.0f,
                40.0f,
                60.0f,
                "Nacisnij E, aby porozmawiac",
                "To moj dom. Rozgosc sie, ale niczego nie ruszaj."),
            MakeHouseEntrance(
                440.0f,
                470.0f,
                80.0f,
                50.0f,
                SceneCoord{0, 0},
                Vector2{120.0f, 360.0f},
                "Nacisnij E, aby wyjsc z domu")
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{11, 10},
        DARKBLUE,
        "Wnetrze Domu 2",
        false,
        {
            MakeWall(120.0f, 120.0f, 720.0f, 40.0f),
            MakeWall(120.0f, 120.0f, 40.0f, 400.0f),
            MakeWall(800.0f, 120.0f, 40.0f, 400.0f),
            MakeWall(120.0f, 480.0f, 300.0f, 40.0f),
            MakeWall(540.0f, 480.0f, 300.0f, 40.0f),
            MakeDecoration(240.0f, 220.0f, 100.0f, 140.0f, SKYBLUE),
            MakeDecoration(600.0f, 240.0f, 120.0f, 60.0f, LIGHTGRAY),
            MakeNpc(
                300.0f,
                240.0f,
                40.0f,
                60.0f,
                "Nacisnij E, aby porozmawiac",
                "Przechowuje tutaj mapy, ale jeszcze zadnej ci nie dam."),
            MakeHouseEntrance(
                440.0f,
                470.0f,
                80.0f,
                50.0f,
                SceneCoord{1, 0},
                Vector2{760.0f, 520.0f},
                "Nacisnij E, aby wyjsc z domu")
        }
    });

    AddScene(SceneDefinition{
        SceneCoord{12, 10},
        DARKGREEN,
        "Wnetrze Domu 3",
        false,
        {
            MakeWall(120.0f, 120.0f, 720.0f, 40.0f),
            MakeWall(120.0f, 120.0f, 40.0f, 400.0f),
            MakeWall(800.0f, 120.0f, 40.0f, 400.0f),
            MakeWall(120.0f, 480.0f, 300.0f, 40.0f),
            MakeWall(540.0f, 480.0f, 300.0f, 40.0f),
            MakeDecoration(260.0f, 240.0f, 160.0f, 50.0f, GREEN),
            MakeDecoration(560.0f, 220.0f, 80.0f, 120.0f, GRAY),
            MakeNpc(
                620.0f,
                260.0f,
                40.0f,
                60.0f,
                "Nacisnij E, aby porozmawiac",
                "Lubie rosliny. Kiedys bedzie tu prawdziwy zielnik."),
            MakeHouseEntrance(
                440.0f,
                470.0f,
                80.0f,
                50.0f,
                SceneCoord{0, 1},
                Vector2{120.0f, 500.0f},
                "Nacisnij E, aby wyjsc z domu")
        }
    });
}

void World::AddScene(const SceneDefinition& definition)
{
    Scene scene{};
    scene.coord = definition.coord;
    scene.backgroundColor = definition.backgroundColor;
    scene.name = definition.name;
    scene.allowEdgeTransitions = definition.allowEdgeTransitions;
    scene.objects = definition.objects;

    m_scenes.push_back(scene);
}