#include "scene.hpp"

#include "config.hpp"

bool SceneCoord::operator==(const SceneCoord& other) const
{
    return x == other.x && y == other.y;
}

Rectangle MakeLeftTransition()
{
    return Rectangle{
        0.0f,
        (static_cast<float>(Config::ScreenHeight) - Config::TransitionLength) * 0.5f,
        Config::TransitionThickness,
        Config::TransitionLength
    };
}

Rectangle MakeRightTransition()
{
    return Rectangle{
        static_cast<float>(Config::ScreenWidth) - Config::TransitionThickness,
        (static_cast<float>(Config::ScreenHeight) - Config::TransitionLength) * 0.5f,
        Config::TransitionThickness,
        Config::TransitionLength
    };
}

Rectangle MakeUpTransition()
{
    return Rectangle{
        (static_cast<float>(Config::ScreenWidth) - Config::TransitionLength) * 0.5f,
        0.0f,
        Config::TransitionLength,
        Config::TransitionThickness
    };
}

Rectangle MakeDownTransition()
{
    return Rectangle{
        (static_cast<float>(Config::ScreenWidth) - Config::TransitionLength) * 0.5f,
        static_cast<float>(Config::ScreenHeight) - Config::TransitionThickness,
        Config::TransitionLength,
        Config::TransitionThickness
    };
}

bool CollidesWithWalls(const Rectangle& rect, const Scene& scene)
{
    for (const Rectangle& wall : scene.walls)
    {
        if (CheckCollisionRecs(rect, wall))
        {
            return true;
        }
    }

    return false;
}

void DrawWalls(const Scene& scene)
{
    for (const Rectangle& wall : scene.walls)
    {
        DrawRectangleRec(wall, BLACK);
    }
}

void DrawSceneInfo(const Scene& scene)
{
    DrawRectangle(12, 12, 340, 110, Fade(BLACK, 0.45f));

    DrawText(scene.name, 24, 24, 28, WHITE);
    DrawText(
        TextFormat("Wspolrzedne sceny: (%d, %d)", scene.coord.x, scene.coord.y),
        24,
        58,
        22,
        WHITE
    );
    DrawText("Czarne bloki = sciany i przejscia", 24, 88, 20, WHITE);
}