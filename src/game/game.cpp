#include "game/game.hpp"

#include <raylib.h>
#include <iostream>

#include "config.hpp"
#include "npc/npcDatabase.hpp"
#include "quest/questSystem.hpp"

#include <optional>

// tworzenie świata i pobranie z config

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
    InitWindow(Config::ScreenWidth, Config::ScreenHeight, "Overworld Prototype - Quest Journal");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // zablokowanie zamykania przez esc

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
    if (IsKeyPressed(KEY_P))
    {
        std::cout << "Scene objects count: " << currentScene->objects.size() << std::endl;

        for (const SceneObject& object : currentScene->objects)
        {
            std::cout
                << "type=" << static_cast<int>(object.type)
                << " x=" << object.rect.x
                << " y=" << object.rect.y
                << " w=" << object.rect.width
                << " h=" << object.rect.height
                << " enemyData=" << (object.enemyData != nullptr ? "yes" : "no")
                << std::endl;
        }
    }
    if (currentScene == nullptr)
    {
        return;
    }

    if(m_battleController.IsActive())
    {
        m_battleController.Update();
        return;
    }

    if (m_dialogueController.IsActive())
    {
        m_dialogueController.Update();
        return;
    }

    m_questJournal.Update(m_gameState);

    if (m_questJournal.IsOpen())
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

    const SceneObject* enemyObject = FindEnemyCollision(m_player.rect, *currentScene);
    if(enemyObject != nullptr && enemyObject->enemyData != nullptr)
    {
        std::cout<<"Encountered enemy: " << enemyObject->enemyData->displayName << std::endl;
        m_battleController.StartBattle(&m_playerStats, enemyObject->enemyData);
        return;
    }

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

            if (interactable->interactionType == InteractionType::Dialogue)
            {
                if (interactable->npcData != nullptr)
                {
                    const DialogueData* resolvedDialogue =
                        NpcDatabase::ResolveDialogue(*interactable->npcData, m_gameState);

                    const char* flagToSet =
                        NpcDatabase::ResolveSetFlagOnUse(*interactable->npcData, m_gameState);

                    if (resolvedDialogue != nullptr)
                    {
                        m_dialogueController.StartDialogue(resolvedDialogue, interactable->npcData);

                        if (flagToSet != nullptr)
                        {
                            m_gameState.SetFlag(flagToSet);
                        }
                    }

                    return;
                }

                if (interactable->dialogueData != nullptr)
                {
                    m_dialogueController.StartDialogue(interactable->dialogueData, interactable->npcData);
                    return;
                }
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

    const QuestData* trackedQuest = m_questJournal.GetTrackedQuest(m_gameState);
    if (trackedQuest != nullptr)
    {
        DrawRectangle(12, 130, 460, 95, Fade(BLACK, 0.55f));
        DrawRectangleLines(12, 130, 460, 95, WHITE);

        DrawText("Sledzone zadanie", 24, 140, 22, YELLOW);
        DrawText(trackedQuest->title, 24, 166, 20, WHITE);

        int y = 190;
        for (const QuestObjective& objective : trackedQuest->objectives)
        {
            const bool completed = QuestSystem::IsObjectiveCompleted(objective, m_gameState);

            DrawText(
                TextFormat("[%c] %s", completed ? 'x' : ' ', objective.description),
                24,
                y,
                18,
                completed ? GREEN : WHITE
            );

            y += 20;
        }
    }

    DrawText("Ruch: WASD / strzalki", 20, Config::ScreenHeight - 60, 24, WHITE);
    DrawText("E = interakcja, J = dziennik zadan", 20, Config::ScreenHeight - 30, 24, WHITE);

    if (!m_dialogueController.IsActive() &&
        !m_questJournal.IsOpen() &&
        interactable != nullptr &&
        interactable->promptText != nullptr)
    {
        DrawText(interactable->promptText, 20, 240, 24, YELLOW);
    }

    m_questJournal.Draw(m_gameState);
    m_dialogueController.Draw();
    m_battleController.Draw();

    EndDrawing();
}