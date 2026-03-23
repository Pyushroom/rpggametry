#include "game.hpp"

#include <raylib.h>

#include "config.hpp"

#include <optional>

Game::Game()
    : m_world{}
    , m_player{
        Rectangle{
            (static_cast<float>(Config::ScreenWidth) - Config::PlayerSize) * 0.5f,
            (static_cast<float>(Config::ScreenHeight) - Config::PlayerSize) * 0.5f,
            Config::PlayerSize,
            Config::PlayerSize
        },
        Config::PlayerSpeed
    }
{
}

int Game::Run()
{
    InitWindow(Config::ScreenWidth, Config::ScreenHeight, "Overworld Prototype - Game Class");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        Update(deltaTime);

        const Scene* currentScene = m_world.FindScene(m_currentCoord);
        if (currentScene == nullptr)
        {
            CloseWindow();
            return 1;
        }

        Draw();
    }

    CloseWindow();
    return 0;
}

void Game::Update(float deltaTime)
{
    const Scene* currentScene = m_world.FindScene(m_currentCoord);
    if (currentScene == nullptr)
    {
        return;
    }

    if (m_transitionCooldown > 0.0f)
    {
        m_transitionCooldown -= deltaTime;
        if (m_transitionCooldown < 0.0f)
        {
            m_transitionCooldown = 0.0f;
        }
    }

    MovePlayer(m_player, *currentScene, deltaTime);

    if (m_transitionCooldown <= 0.0f)
    {
        const std::optional<Direction> transition =
            m_world.CheckTransitionCollision(m_player, m_currentCoord);

        if (transition.has_value())
        {
            m_currentCoord = m_world.GetNextCoord(m_currentCoord, *transition);
            SetPlayerPositionAfterTransition(m_player, *transition);
            m_transitionCooldown = Config::TransitionCooldownTime;
        }
    }

    if (IsKeyPressed(KEY_E))
    {
        const SceneObject* interactable =
            FindInteractableObject(m_player.rect, *currentScene);

        if (interactable != nullptr)
        {
            if (interactable->type == SceneObjectType::HouseEntrance)
            {
                // na razie debug:
                TraceLog(LOG_INFO, "Wszedles do domu!");
            }
        }
    }
}

void Game::Draw() const
{
    const Scene* currentScene = m_world.FindScene(m_currentCoord);
    if (currentScene == nullptr)
    {
        return;
    }

    const SceneObject* interactable =
    FindInteractableObject(m_player.rect, *currentScene);
    if (interactable != nullptr)
    {
        DrawText("Nacisnij E", 20, 100, 24, YELLOW);
    }

    BeginDrawing();
    ClearBackground(currentScene->backgroundColor);

    m_world.DrawAvailableTransitions(m_currentCoord);
    DrawSceneObjects(*currentScene);
    DrawRectangleRec(m_player.rect, DARKGRAY);
    DrawSceneInfo(*currentScene);

    DrawText("Ruch: WASD / strzalki", 20, Config::ScreenHeight - 60, 24, WHITE);
    DrawText("Config.hpp trzyma wspolne stale", 20, Config::ScreenHeight - 30, 24, WHITE);

    EndDrawing();
}