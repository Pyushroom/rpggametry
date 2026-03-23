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
    InitWindow(Config::ScreenWidth, Config::ScreenHeight, "Overworld Prototype - Multi Page Dialogue");
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

void Game::ResetDialogueState()
{
    m_dialogueMode = DialogueMode::Hidden;
    m_activeDialogue = nullptr;
    m_currentPages.clear();
    m_currentPageIndex = 0;
    m_selectedChoiceIndex = 0;
}

void Game::StartDialogue(const DialogueData* dialogueData)
{
    ResetDialogueState();

    if (dialogueData == nullptr)
    {
        return;
    }

    m_activeDialogue = dialogueData;
    m_currentPages = dialogueData->openingPages;
    m_currentPageIndex = 0;

    if (!m_currentPages.empty())
    {
        m_dialogueMode = DialogueMode::OpeningPages;
    }
    else if (!dialogueData->choices.empty())
    {
        m_dialogueMode = DialogueMode::ChoiceSelection;
    }
}

void Game::AdvanceDialoguePage()
{
    if (m_currentPages.empty())
    {
        return;
    }

    ++m_currentPageIndex;

    if (m_currentPageIndex < static_cast<int>(m_currentPages.size()))
    {
        return;
    }

    if (m_dialogueMode == DialogueMode::OpeningPages)
    {
        if (m_activeDialogue != nullptr && !m_activeDialogue->choices.empty())
        {
            m_dialogueMode = DialogueMode::ChoiceSelection;
            m_currentPages.clear();
            m_currentPageIndex = 0;
            return;
        }

        ResetDialogueState();
        return;
    }

    if (m_dialogueMode == DialogueMode::ResponsePages)
    {
        ResetDialogueState();
    }
}

void Game::Update(float deltaTime)
{
    const Scene* currentScene = m_world.FindScene(m_currentCoord);
    if (currentScene == nullptr)
    {
        return;
    }

    if (m_dialogueMode != DialogueMode::Hidden)
    {
        if (m_dialogueMode == DialogueMode::OpeningPages ||
            m_dialogueMode == DialogueMode::ResponsePages)
        {
            if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
            {
                AdvanceDialoguePage();
            }

            return;
        }

        if (m_dialogueMode == DialogueMode::ChoiceSelection)
        {
            if (m_activeDialogue == nullptr)
            {
                ResetDialogueState();
                return;
            }

            const int choiceCount = static_cast<int>(m_activeDialogue->choices.size());

            if (choiceCount <= 0)
            {
                ResetDialogueState();
                return;
            }

            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
            {
                --m_selectedChoiceIndex;
                if (m_selectedChoiceIndex < 0)
                {
                    m_selectedChoiceIndex = choiceCount - 1;
                }
            }

            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
            {
                ++m_selectedChoiceIndex;
                if (m_selectedChoiceIndex >= choiceCount)
                {
                    m_selectedChoiceIndex = 0;
                }
            }

            if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
            {
                m_currentPages =
                    m_activeDialogue->choices[static_cast<std::size_t>(m_selectedChoiceIndex)].npcResponsePages;
                m_currentPageIndex = 0;

                if (!m_currentPages.empty())
                {
                    m_dialogueMode = DialogueMode::ResponsePages;
                }
                else
                {
                    ResetDialogueState();
                }
            }

            return;
        }
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
                StartDialogue(interactable->dialogueData);
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

    if (m_dialogueMode == DialogueMode::Hidden &&
        interactable != nullptr &&
        interactable->promptText != nullptr)
    {
        DrawText(interactable->promptText, 20, 130, 24, YELLOW);
    }

    if (m_dialogueMode != DialogueMode::Hidden && m_activeDialogue != nullptr)
    {
        DrawRectangle(40, Config::ScreenHeight - 240, Config::ScreenWidth - 80, 180, Fade(BLACK, 0.85f));
        DrawRectangleLines(40, Config::ScreenHeight - 240, Config::ScreenWidth - 80, 180, WHITE);

        DrawText(m_activeDialogue->speakerName, 60, Config::ScreenHeight - 220, 28, YELLOW);

        if ((m_dialogueMode == DialogueMode::OpeningPages || m_dialogueMode == DialogueMode::ResponsePages) &&
            !m_currentPages.empty() &&
            m_currentPageIndex >= 0 &&
            m_currentPageIndex < static_cast<int>(m_currentPages.size()))
        {
            DrawText(m_currentPages[static_cast<std::size_t>(m_currentPageIndex)],
                     60,
                     Config::ScreenHeight - 180,
                     24,
                     WHITE);

            DrawText(TextFormat("Strona %d / %d",
                                m_currentPageIndex + 1,
                                static_cast<int>(m_currentPages.size())),
                     60,
                     Config::ScreenHeight - 100,
                     20,
                     LIGHTGRAY);

            DrawText("Nacisnij E / Enter / Spacje, aby kontynuowac",
                     220,
                     Config::ScreenHeight - 100,
                     20,
                     LIGHTGRAY);
        }
        else if (m_dialogueMode == DialogueMode::ChoiceSelection)
        {
            if (!m_activeDialogue->openingPages.empty())
            {
                DrawText(m_activeDialogue->openingPages.back(),
                         60,
                         Config::ScreenHeight - 190,
                         22,
                         WHITE);
            }

            const int startY = Config::ScreenHeight - 145;
            for (int index = 0; index < static_cast<int>(m_activeDialogue->choices.size()); ++index)
            {
                const char* prefix = (index == m_selectedChoiceIndex) ? "> " : "  ";
                DrawText(
                    TextFormat("%s%s",
                               prefix,
                               m_activeDialogue->choices[static_cast<std::size_t>(index)].playerText),
                    60,
                    startY + index * 24,
                    20,
                    (index == m_selectedChoiceIndex) ? YELLOW : WHITE);
            }
        }
    }

    EndDrawing();
}