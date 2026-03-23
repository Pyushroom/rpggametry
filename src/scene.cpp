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

bool CollidesWithBlockingObjects(const Rectangle& rect, const Scene& scene)
{
    for (const SceneObject& object : scene.objects)
    {
        if (!object.blocksMovement)
        {
            continue;
        }

        if (CheckCollisionRecs(rect, object.rect))
        {
            return true;
        }
    }

    return false;
}

void DrawSceneObjects(const Scene& scene)
{
    for (const SceneObject& object : scene.objects)
    {
        DrawRectangleRec(object.rect, object.color);

        if (object.type == SceneObjectType::Ladder)
        {
            const int rungCount = 4;
            const float rungHeight = object.rect.height / static_cast<float>(rungCount + 1);

            for (int index = 1; index <= rungCount; ++index)
            {
                const float y = object.rect.y + rungHeight * static_cast<float>(index);
                DrawLineEx(
                    Vector2{object.rect.x, y},
                    Vector2{object.rect.x + object.rect.width, y},
                    2.0f,
                    DARKBROWN
                );
            }
        }
    }
}

void DrawSceneInfo(const Scene& scene)
{
    DrawRectangle(12, 12, 380, 110, Fade(BLACK, 0.45f));

    DrawText(scene.name, 24, 24, 28, WHITE);
    DrawText(
        TextFormat("Wspolrzedne sceny: (%d, %d)", scene.coord.x, scene.coord.y),
        24,
        58,
        22,
        WHITE
    );
    DrawText("Obiekty: wall, rock, ladder, decoration", 24, 88, 20, WHITE);
}

SceneObject MakeWall(Rectangle rect)
{
    return SceneObject{
        SceneObjectType::Wall,
        rect,
        true,
        DARKGRAY
    };
}

SceneObject MakeRock(Rectangle rect)
{
    return SceneObject{
        SceneObjectType::Rock,
        rect,
        true,
        GRAY
    };
}

SceneObject MakeLadder(Rectangle rect)
{
    return SceneObject{
        SceneObjectType::Ladder,
        rect,
        false,
        BROWN
    };
}

SceneObject MakeDecoration(Rectangle rect, Color color)
{
    return SceneObject{
        SceneObjectType::Decoration,
        rect,
        false,
        color
    };
}