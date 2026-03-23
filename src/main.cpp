#include <raylib.h>

#include "player.hpp"
#include "scene.hpp"
#include "world.hpp"

namespace
{
constexpr int ScreenWidth = 960;
constexpr int ScreenHeight = 640;
constexpr float PlayerSize = 32.0f;
constexpr float PlayerSpeed = 220.0f;
constexpr float TransitionCooldownTime = 0.20f;
}

int main()
{
    InitWindow(ScreenWidth, ScreenHeight, "Overworld Prototype - Split Files");
    SetTargetFPS(60);

    World world{};

    Player player{
        Rectangle{
            (static_cast<float>(ScreenWidth) - PlayerSize) * 0.5f,
            (static_cast<float>(ScreenHeight) - PlayerSize) * 0.5f,
            PlayerSize,
            PlayerSize
        },
        PlayerSpeed
    };

    SceneCoord currentCoord{0, 0};
    float transitionCooldown = 0.0f;

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        const Scene* currentScene = world.FindScene(currentCoord);
        if (currentScene == nullptr)
        {
            CloseWindow();
            return 1;
        }

        if (transitionCooldown > 0.0f)
        {
            transitionCooldown -= deltaTime;
            if (transitionCooldown < 0.0f)
            {
                transitionCooldown = 0.0f;
            }
        }

        MovePlayer(player, *currentScene, deltaTime);

        if (transitionCooldown <= 0.0f)
        {
            const std::optional<Direction> transition =
                world.CheckTransitionCollision(player, currentCoord);

            if (transition.has_value())
            {
                currentCoord = world.GetNextCoord(currentCoord, *transition);
                SetPlayerPositionAfterTransition(player, *transition);
                transitionCooldown = TransitionCooldownTime;

                currentScene = world.FindScene(currentCoord);
                if (currentScene == nullptr)
                {
                    CloseWindow();
                    return 1;
                }
            }
        }

        BeginDrawing();
        ClearBackground(currentScene->backgroundColor);

        world.DrawAvailableTransitions(currentCoord);
        DrawWalls(*currentScene);
        DrawRectangleRec(player.rect, DARKGRAY);
        DrawSceneInfo(*currentScene);

        DrawText("Ruch: WASD / strzalki", 20, ScreenHeight - 60, 24, WHITE);
        DrawText("Projekt rozdzielony na pliki", 20, ScreenHeight - 30, 24, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}