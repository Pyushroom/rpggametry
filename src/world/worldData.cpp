#include "world/worldData.hpp"
#include "npc/npcDatabase.hpp"


namespace WorldData
{
std::vector<Scene> CreateScenes()
{
    std::vector<Scene> scenes;

    scenes.push_back(Scene{
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
                &NpcDatabase::GetTravelerNpc()),
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

    scenes.push_back(Scene{
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
                &NpcDatabase::GetLadderGuideNpc()),
        }
    });

    scenes.push_back(Scene{
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
                &NpcDatabase::GetGuardNpc()),
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

    scenes.push_back(Scene{
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
                &NpcDatabase::GetHillWatcherNpc())
        }
    });

    scenes.push_back(Scene{
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

    scenes.push_back(Scene{
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
                &NpcDatabase::GetHouseOwnerNpc()),
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

    scenes.push_back(Scene{
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
                &NpcDatabase::GetCartographerNpc()),
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

    scenes.push_back(Scene{
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
                &NpcDatabase::GetHerbalistNpc()),
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

    return scenes;
}
}