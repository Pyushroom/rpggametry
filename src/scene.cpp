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

const SceneObject* FindInteractableObjectNearby(const Rectangle& playerRect, const Scene& scene)
{
    constexpr float interactionRange = 12.0f;

    for (const SceneObject& object : scene.objects)
    {
        if (!object.isInteractable)
        {
            continue;
        }

        Rectangle expandedRect{
            object.rect.x - interactionRange,
            object.rect.y - interactionRange,
            object.rect.width + interactionRange * 2.0f,
            object.rect.height + interactionRange * 2.0f
        };

        if (CheckCollisionRecs(playerRect, expandedRect))
        {
            return &object;
        }
    }

    return nullptr;
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
        else if (object.type == SceneObjectType::Tree)
        {
            const Rectangle trunk{
                object.rect.x + object.rect.width * 0.4f,
                object.rect.y + object.rect.height * 0.7f,
                object.rect.width * 0.2f,
                object.rect.height * 0.3f
            };

            DrawRectangleRec(trunk, BROWN);
        }
        else if (object.type == SceneObjectType::HouseEntrance)
        {
            DrawRectangleLinesEx(object.rect, 2.0f, BLACK);
        }
        else if (object.type == SceneObjectType::Npc)
        {
            DrawRectangleLinesEx(object.rect, 2.0f, BLACK);
        }
    }
}

void DrawSceneInfo(const Scene& scene)
{
    DrawRectangle(12, 12, 420, 110, Fade(BLACK, 0.45f));

    DrawText(scene.name, 24, 24, 28, WHITE);
    DrawText(
        TextFormat("Wspolrzedne sceny: (%d, %d)", scene.coord.x, scene.coord.y),
        24,
        58,
        22,
        WHITE
    );
    DrawText("E = interakcja z obiektami", 24, 88, 20, WHITE);
}

SceneObject MakeWall(float x, float y, float width, float height)
{
    return SceneObject{
        SceneObjectType::Wall,
        Rectangle{x, y, width, height},
        true,
        false,
        DARKGRAY,
        InteractionType::None,
        nullptr,
        nullptr,
        false,
        SceneCoord{},
        Vector2{}
    };
}

SceneObject MakeRock(float x, float y, float width, float height)
{
    return SceneObject{
        SceneObjectType::Rock,
        Rectangle{x, y, width, height},
        true,
        false,
        GRAY,
        InteractionType::None,
        nullptr,
        nullptr,
        false,
        SceneCoord{},
        Vector2{}
    };
}

SceneObject MakeLadder(
    float x,
    float y,
    float width,
    float height,
    Vector2 targetPlayerPosition,
    const char* promptText)
{
    return SceneObject{
        SceneObjectType::Ladder,
        Rectangle{x, y, width, height},
        false,
        true,
        BROWN,
        InteractionType::Teleport,
        promptText,
        nullptr,
        false,
        SceneCoord{},
        targetPlayerPosition
    };
}

SceneObject MakeDecoration(float x, float y, float width, float height, Color color)
{
    return SceneObject{
        SceneObjectType::Decoration,
        Rectangle{x, y, width, height},
        false,
        false,
        color,
        InteractionType::None,
        nullptr,
        nullptr,
        false,
        SceneCoord{},
        Vector2{}
    };
}

SceneObject MakeBush(float x, float y, float width, float height)
{
    return SceneObject{
        SceneObjectType::Bush,
        Rectangle{x, y, width, height},
        true,
        false,
        DARKGREEN,
        InteractionType::None,
        nullptr,
        nullptr,
        false,
        SceneCoord{},
        Vector2{}
    };
}

SceneObject MakeTree(float x, float y, float width, float height)
{
    return SceneObject{
        SceneObjectType::Tree,
        Rectangle{x, y, width, height},
        true,
        false,
        GREEN,
        InteractionType::None,
        nullptr,
        nullptr,
        false,
        SceneCoord{},
        Vector2{}
    };
}

SceneObject MakeNpc(
    float x,
    float y,
    float width,
    float height,
    const char* promptText,
    const char* dialogText)
{
    return SceneObject{
        SceneObjectType::Npc,
        Rectangle{x, y, width, height},
        true,
        true,
        VIOLET,
        InteractionType::Dialogue,
        promptText,
        dialogText,
        false,
        SceneCoord{},
        Vector2{}
    };
}

SceneObject MakeHouseEntrance(
    float x,
    float y,
    float width,
    float height,
    SceneCoord targetSceneCoord,
    Vector2 targetPlayerPosition,
    const char* promptText)
{
    return SceneObject{
        SceneObjectType::HouseEntrance,
        Rectangle{x, y, width, height},
        false,
        true,
        MAROON,
        InteractionType::Teleport,
        promptText,
        nullptr,
        true,
        targetSceneCoord,
        targetPlayerPosition
    };
}