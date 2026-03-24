#include "game/Game.hpp"

#include <raylib.h>

#include "Config.hpp"

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
    InitWindow(Config::ScreenWidth, Config::ScreenHeight, "Overworld Prototype - Dialogue Controller");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

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

    if (m_dialogueController.IsActive())
    {
        m_dialogueController.Update();
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

    if (IsKeyPressed(KEY_E))
    {
        const SceneObject* interactable =
            FindInteractableObjectNearby(m_player.rect, *currentScene);

        if (interactable != nullptr)
        {
            if (interactable->interactionType == InteractionType::Teleport)
            {
                if (interactable->hasTargetScene)
                {
                    m_currentCoord = interactable->targetSceneCoord;
                }

                SetPlayerPosition(m_player, interactable->targetPlayerPosition);
                m_transitionCooldown = Config::TransitionCooldownTime;
                return;
            }

            if (interactable->interactionType == InteractionType::Dialogue &&
                interactable->dialogueData != nullptr)
            {
                m_dialogueController.StartDialogue(interactable->dialogueData);
                return;
            }
        }
    }

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
}

void Game::Draw() const
{
    const Scene* currentScene = m_world.FindScene(m_currentCoord);
    if (currentScene == nullptr)
    {
        return;
    }

    const SceneObject* interactable =
        FindInteractableObjectNearby(m_player.rect, *currentScene);

    BeginDrawing();
    ClearBackground(currentScene->backgroundColor);

    m_world.DrawAvailableTransitions(m_currentCoord);
    DrawSceneObjects(*currentScene);
    DrawRectangleRec(m_player.rect, DARKGRAY);
    DrawSceneInfo(*currentScene);

    DrawText("Ruch: WASD / strzalki", 20, Config::ScreenHeight - 60, 24, WHITE);
    DrawText("E = interakcja", 20, Config::ScreenHeight - 30, 24, WHITE);

    if (!m_dialogueController.IsActive() &&
        interactable != nullptr &&
        interactable->promptText != nullptr)
    {
        DrawText(interactable->promptText, 20, 130, 24, YELLOW);
    }

    m_dialogueController.Draw();

    EndDrawing();
}