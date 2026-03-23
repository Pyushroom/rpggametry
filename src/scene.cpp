#include "scene.hpp"

bool SceneCoord::operator==(const SceneCoord& other) const
{
    return x == other.x && y == other.y;
}

Rectangle MakeLeftTransition()
{
    constexpr int screenHeight = 640;
    constexpr float transitionLength = 120.0f;
    constexpr float transitionThickness = 24.0f;

    return Rectangle{
        0.0f,
        (static_cast<float>(screenHeight) - transitionLength) * 0.5f,
        transitionThickness,
        transitionLength
    };
}

Rectangle MakeRightTransition()
{
    constexpr int screenWidth = 960;
    constexpr int screenHeight = 640;
    constexpr float transitionLength = 120.0f;
    constexpr float transitionThickness = 24.0f;

    return Rectangle{
        static_cast<float>(screenWidth) - transitionThickness,
        (static_cast<float>(screenHeight) - transitionLength) * 0.5f,
        transitionThickness,
        transitionLength
    };
}

Rectangle MakeUpTransition()
{
    constexpr int screenWidth = 960;
    constexpr float transitionLength = 120.0f;
    constexpr float transitionThickness = 24.0f;

    return Rectangle{
        (static_cast<float>(screenWidth) - transitionLength) * 0.5f,
        0.0f,
        transitionLength,
        transitionThickness
    };
}

Rectangle MakeDownTransition()
{
    constexpr int screenWidth = 960;
    constexpr int screenHeight = 640;
    constexpr float transitionLength = 120.0f;
    constexpr float transitionThickness = 24.0f;

    return Rectangle{
        (static_cast<float>(screenWidth) - transitionLength) * 0.5f,
        static_cast<float>(screenHeight) - transitionThickness,
        transitionLength,
        transitionThickness
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