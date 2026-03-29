#include "scene/scene.hpp"

#include <raylib.h>

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

        if (object.type == SceneObjectType::Enemy && object.isDefeated)
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

std::optional<std::size_t> FindEnemyCollisionIndex(const Rectangle& playerRect, const Scene& scene)
{
    for (std::size_t index = 0; index < scene.objects.size(); ++index)
    {
        const SceneObject& object = scene.objects[index];

        if (object.type != SceneObjectType::Enemy)
        {
            continue;
        }

        if (object.enemyData == nullptr)
        {
            continue;
        }

        if (object.isDefeated)
        {
            continue;
        }

        if (CheckCollisionRecs(playerRect, object.rect))
        {
            return index;
        }
    }

    return std::nullopt;
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
        if (object.type == SceneObjectType::Enemy && object.isDefeated)
        {
            continue;
        }

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
        else if (object.type == SceneObjectType::HouseEntrance || object.type == SceneObjectType::Npc)
        {
            DrawRectangleLinesEx(object.rect, 2.0f, BLACK);
        }
        else if (object.type == SceneObjectType::Enemy)
        {
            DrawRectangleLinesEx(object.rect, 2.0f, BLACK);
            DrawText(
                "ENEMY",
                static_cast<int>(object.rect.x),
                static_cast<int>(object.rect.y - 18.0f),
                18,
                WHITE
            );
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
    SceneObject object{};
    object.type = SceneObjectType::Wall;
    object.rect = Rectangle{x, y, width, height};
    object.blocksMovement = true;
    object.isInteractable = false;
    object.color = DARKGRAY;
    object.interactionType = InteractionType::None;
    return object;
}

SceneObject MakeRock(float x, float y, float width, float height)
{
    SceneObject object{};
    object.type = SceneObjectType::Rock;
    object.rect = Rectangle{x, y, width, height};
    object.blocksMovement = true;
    object.isInteractable = false;
    object.color = GRAY;
    object.interactionType = InteractionType::None;
    return object;
}

SceneObject MakeLadder(
    float x,
    float y,
    float width,
    float height,
    Vector2 targetPlayerPosition,
    const char* promptText)
{
    SceneObject object{};
    object.type = SceneObjectType::Ladder;
    object.rect = Rectangle{x, y, width, height};
    object.blocksMovement = false;
    object.isInteractable = true;
    object.color = BROWN;
    object.interactionType = InteractionType::Teleport;
    object.promptText = promptText;
    object.targetPlayerPosition = targetPlayerPosition;
    return object;
}

SceneObject MakeDecoration(float x, float y, float width, float height, Color color)
{
    SceneObject object{};
    object.type = SceneObjectType::Decoration;
    object.rect = Rectangle{x, y, width, height};
    object.blocksMovement = false;
    object.isInteractable = false;
    object.color = color;
    object.interactionType = InteractionType::None;
    return object;
}

SceneObject MakeBush(float x, float y, float width, float height)
{
    SceneObject object{};
    object.type = SceneObjectType::Bush;
    object.rect = Rectangle{x, y, width, height};
    object.blocksMovement = true;
    object.isInteractable = false;
    object.color = DARKGREEN;
    object.interactionType = InteractionType::None;
    return object;
}

SceneObject MakeTree(float x, float y, float width, float height)
{
    SceneObject object{};
    object.type = SceneObjectType::Tree;
    object.rect = Rectangle{x, y, width, height};
    object.blocksMovement = true;
    object.isInteractable = false;
    object.color = GREEN;
    object.interactionType = InteractionType::None;
    return object;
}

SceneObject MakeNpc(
    float x,
    float y,
    float width,
    float height,
    const NpcData* npcData)
{
    SceneObject object{};
    object.type = SceneObjectType::Npc;
    object.rect = Rectangle{x, y, width, height};
    object.blocksMovement = true;
    object.isInteractable = true;
    object.color = npcData != nullptr ? npcData->color : VIOLET;
    object.interactionType = InteractionType::Dialogue;
    object.promptText = npcData != nullptr ? npcData->promptText : nullptr;
    object.npcData = npcData;
    return object;
}

SceneObject MakeEnemy(
    float x,
    float y,
    float width,
    float height,
    const EnemyData* enemyData)
{
    SceneObject object{};
    object.type = SceneObjectType::Enemy;
    object.rect = Rectangle{x, y, width, height};
    object.blocksMovement = false;
    object.isInteractable = false;
    object.color = RED;
    object.interactionType = InteractionType::None;
    object.enemyData = enemyData;
    object.isDefeated = false;
    return object;
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
    SceneObject object{};
    object.type = SceneObjectType::HouseEntrance;
    object.rect = Rectangle{x, y, width, height};
    object.blocksMovement = false;
    object.isInteractable = true;
    object.color = MAROON;
    object.interactionType = InteractionType::Teleport;
    object.promptText = promptText;
    object.hasTargetScene = true;
    object.targetSceneCoord = targetSceneCoord;
    object.targetPlayerPosition = targetPlayerPosition;
    return object;
}