#include "game/game.hpp"

#include <raylib.h>

#include "config.hpp"
#include "npc/npcDatabase.hpp"
#include "quest/questSystem.hpp"

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
    InitWindow(Config::ScreenWidth, Config::ScreenHeight, "Overworld Prototype - Battle Prototype");
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

    if (m_enemyEncounterCooldown > 0.0f)
    {
        m_enemyEncounterCooldown -= deltaTime;
        if (m_enemyEncounterCooldown < 0.0f)
        {
            m_enemyEncounterCooldown = 0.0f;
        }
    }

    if (m_battleController.IsActive())
    {
        m_battleController.Update();

        if (!m_battleController.IsActive())
        {
            const BattleResult result = m_battleController.GetResult();

             if (result == BattleResult::Victory)
            {
                Scene* battleScene = m_world.FindScene(m_battleSceneCoord);
                if (battleScene != nullptr && m_activeEnemyIndex.has_value())
                {
                    const std::size_t index = *m_activeEnemyIndex;
                    if (index < battleScene->objects.size())
                    {
                        battleScene->objects[index].isDefeated = true;

                        if (battleScene->objects[index].enemyData != nullptr)
                        {
                            m_playerStats.AddExperience(
                                battleScene->objects[index].enemyData->expReward
                            );
                        }
                    }
                }

                m_enemyEncounterCooldown = 0.5f;
            }
            else if (result == BattleResult::Escape)
            {
                Scene* battleScene = m_world.FindScene(m_battleSceneCoord);
                if (battleScene != nullptr && m_activeEnemyIndex.has_value())
                {
                    const std::size_t index = *m_activeEnemyIndex;
                    if (index < battleScene->objects.size())
                    {
                        const Rectangle enemyRect = battleScene->objects[index].rect;

                        Vector2 escapePosition{
                            enemyRect.x - m_player.rect.width - 8.0f,
                            m_player.rect.y
                        };

                        if (escapePosition.x < 0.0f)
                        {
                            escapePosition.x = enemyRect.x + enemyRect.width + 8.0f;
                        }

                        SetPlayerPosition(m_player, escapePosition);
                    }
                }

                m_enemyEncounterCooldown = 0.5f;
            }
            else if (result == BattleResult::Defeat)
            {
                m_enemyEncounterCooldown = 0.5f;
            }

            m_activeEnemyIndex.reset();
        }

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

    if (m_enemyEncounterCooldown <= 0.0f)
    {
        const std::optional<std::size_t> enemyIndex =
            FindEnemyCollisionIndex(m_player.rect, *currentScene);

        if (enemyIndex.has_value())
        {
            const SceneObject& enemyObject = currentScene->objects[*enemyIndex];

            if (enemyObject.enemyData != nullptr)
            {
                m_activeEnemyIndex = enemyIndex;
                m_battleSceneCoord = m_currentCoord;
                m_battleController.StartBattle(&m_playerStats, enemyObject.enemyData);
                return;
            }
        }
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

    DrawText(
        TextFormat("Lvl %d  EXP: %d/%d",
            m_playerStats.level,
            m_playerStats.experience,
            m_playerStats.expToNextLevel),
        700,
        40,
        22,
        WHITE);

    if (!m_dialogueController.IsActive() &&
        !m_questJournal.IsOpen() &&
        !m_battleController.IsActive() &&
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