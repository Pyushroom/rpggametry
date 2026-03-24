#include "game/game.hpp"

#include <raylib.h>

#include "config.hpp"

#include <optional>
#include <string>
#include <vector>

namespace
{
constexpr int DialogueBoxX = 40;
constexpr int DialogueBoxY = Config::ScreenHeight - 250;
constexpr int DialogueBoxWidth = Config::ScreenWidth - 80;
constexpr int DialogueBoxHeight = 190;
constexpr int DialogueTextLeft = 60;
constexpr int DialogueTextWidth = DialogueBoxWidth - 40;

constexpr int DialogueBodyFontSize = 24;
constexpr int DialogueChoiceFontSize = 20;
constexpr int DialogueOpeningFontSize = 22;

constexpr int DialogueMaxLinesPerPage = 3;
constexpr int DialogueResponseMaxLinesPerPage = 3;
}
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
    InitWindow(Config::ScreenWidth, Config::ScreenHeight, "Overworld Prototype - Dialogue System");
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

void Game::CloseDialogue()
{
    ResetDialogueState();
}

void Game::ResetDialogueState()
{
    m_dialogueMode = DialogueMode::Hidden;
    m_activeDialogue = nullptr;
    m_currentPages.clear();
    m_currentPageIndex = 0;
    m_selectedChoiceIndex = 0;
    m_selectedChoiceText = nullptr;
}

std::vector<std::string> Game::WrapTextToLines(const char* text, int fontSize, int maxWidth) const
{
    if (text == nullptr)
    {
        return {};
    }

    const std::string input{text};
    std::vector<std::string> lines;
    std::string currentLine;
    std::string currentWord;

    auto pushCurrentLine = [&]()
    {
        if (!currentLine.empty())
        {
            lines.push_back(currentLine);
            currentLine.clear();
        }
    };

    auto flushWord = [&]()
    {
        if (currentWord.empty())
        {
            return;
        }

        std::string candidate = currentLine.empty()
            ? currentWord
            : currentLine + " " + currentWord;

        if (MeasureText(candidate.c_str(), fontSize) <= maxWidth)
        {
            currentLine = candidate;
        }
        else
        {
            if (!currentLine.empty())
            {
                lines.push_back(currentLine);
            }

            currentLine = currentWord;
        }

        currentWord.clear();
    };

    for (char ch : input)
    {
        if (ch == ' ')
        {
            flushWord();
        }
        else if (ch == '\n')
        {
            flushWord();
            pushCurrentLine();
        }
        else
        {
            currentWord += ch;
        }
    }

    flushWord();
    pushCurrentLine();

    return lines;
}

std::vector<std::string> Game::PaginateText(
    const char* text,
    int fontSize,
    int maxWidth,
    int maxLinesPerPage) const
{
    const std::vector<std::string> lines = WrapTextToLines(text, fontSize, maxWidth);
    std::vector<std::string> pages;

    if (lines.empty())
    {
        return pages;
    }

    std::string currentPage;
    int currentLineCount = 0;

    for (const std::string& line : lines)
    {
        if (currentLineCount == maxLinesPerPage)
        {
            pages.push_back(currentPage);
            currentPage.clear();
            currentLineCount = 0;
        }

        if (!currentPage.empty())
        {
            currentPage += '\n';
        }

        currentPage += line;
        ++currentLineCount;
    }

    if (!currentPage.empty())
    {
        pages.push_back(currentPage);
    }

    return pages;
}

void Game::SetDialoguePagesFromText(
    const char* text,
    int fontSize,
    int maxWidth,
    int maxLinesPerPage)
{
    m_currentPages = PaginateText(text, fontSize, maxWidth, maxLinesPerPage);
    m_currentPageIndex = 0;
}

void Game::StartDialogue(const DialogueData* dialogueData)
{
    ResetDialogueState();

    if (dialogueData == nullptr)
    {
        return;
    }

    m_activeDialogue = dialogueData;
    m_selectedChoiceIndex = 0;

    if (!dialogueData->openingPages.empty())
    {
        SetDialoguePagesFromText(
            dialogueData->openingPages.front(),
            DialogueBodyFontSize,
            DialogueTextWidth,
            DialogueMaxLinesPerPage
        );

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

        CloseDialogue();
        return;
    }

    if (m_dialogueMode == DialogueMode::ResponsePages)
    {
        if (m_activeDialogue != nullptr && !m_activeDialogue->choices.empty())
        {
            m_dialogueMode = DialogueMode::ChoiceSelection;
            m_currentPages.clear();
            m_currentPageIndex = 0;
            return;
        }

        CloseDialogue();
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
        if (IsKeyPressed(KEY_ESCAPE))
        {
            CloseDialogue();
            return;
        }

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
                CloseDialogue();
                return;
            }

            const int choiceCount = static_cast<int>(m_activeDialogue->choices.size());

            if (choiceCount <= 0)
            {
                CloseDialogue();
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
                const DialogueChoice& choice =
                    m_activeDialogue->choices[static_cast<std::size_t>(m_selectedChoiceIndex)];

                m_selectedChoiceText = choice.playerText;

                std::string joinedResponse;
                for (std::size_t index = 0; index < choice.npcResponsePages.size(); ++index)
                {
                    if (index > 0)
                    {
                        joinedResponse += '\n';
                    }

                    joinedResponse += choice.npcResponsePages[index];
                }

                SetDialoguePagesFromText(
                    joinedResponse.c_str(),
                    DialogueBodyFontSize,
                    DialogueTextWidth,
                    DialogueResponseMaxLinesPerPage
                );

                if (!m_currentPages.empty())
                {
                    m_dialogueMode = DialogueMode::ResponsePages;
                }
                else
                {
                    m_dialogueMode = DialogueMode::ChoiceSelection;
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
        DrawRectangle(DialogueBoxX, DialogueBoxY, DialogueBoxWidth, DialogueBoxHeight, Fade(BLACK, 0.88f));
        DrawRectangleLines(DialogueBoxX, DialogueBoxY, DialogueBoxWidth, DialogueBoxHeight, WHITE);

        DrawText(m_activeDialogue->speakerName, DialogueTextLeft, Config::ScreenHeight - 228, 28, YELLOW);

        if ((m_dialogueMode == DialogueMode::OpeningPages || m_dialogueMode == DialogueMode::ResponsePages) &&
            !m_currentPages.empty() &&
            m_currentPageIndex >= 0 &&
            m_currentPageIndex < static_cast<int>(m_currentPages.size()))
        {
            if (m_dialogueMode == DialogueMode::ResponsePages && m_selectedChoiceText != nullptr)
            {
                DrawText(TextFormat("Ty: %s", m_selectedChoiceText),
                         DialogueTextLeft,
                         Config::ScreenHeight - 195,
                         20,
                         LIGHTGRAY);

                DrawText(m_currentPages[static_cast<std::size_t>(m_currentPageIndex)].c_str(),
                         DialogueTextLeft,
                         Config::ScreenHeight - 165,
                         DialogueBodyFontSize,
                         WHITE);
            }
            else
            {
                DrawText(m_currentPages[static_cast<std::size_t>(m_currentPageIndex)].c_str(),
                         DialogueTextLeft,
                         Config::ScreenHeight - 180,
                         DialogueBodyFontSize,
                         WHITE);
            }

            DrawText(TextFormat("Strona %d / %d",
                                m_currentPageIndex + 1,
                                static_cast<int>(m_currentPages.size())),
                     DialogueTextLeft,
                     Config::ScreenHeight - 95,
                     20,
                     LIGHTGRAY);

            DrawText("E / Enter / Spacja = dalej, Esc = zamknij",
                     220,
                     Config::ScreenHeight - 95,
                     20,
                     LIGHTGRAY);
        }
        else if (m_dialogueMode == DialogueMode::ChoiceSelection)
        {
            const int startY = Config::ScreenHeight - 185;

            if (m_activeDialogue->openingPages.empty())
            {
                DrawText("Wybierz odpowiedz:", DialogueTextLeft, startY, DialogueOpeningFontSize, WHITE);
            }
            else
            {
                const std::vector<std::string> wrappedOpening =
                    PaginateText(
                        m_activeDialogue->openingPages.back(),
                        DialogueOpeningFontSize,
                        DialogueTextWidth,
                        2
                    );

                if (!wrappedOpening.empty())
                {
                    DrawText(wrappedOpening.front().c_str(),
                             DialogueTextLeft,
                             startY,
                             DialogueOpeningFontSize,
                             WHITE);
                }
            }

            const int optionsY = Config::ScreenHeight - 145;
            for (int index = 0; index < static_cast<int>(m_activeDialogue->choices.size()); ++index)
            {
                const char* prefix = (index == m_selectedChoiceIndex) ? "> " : "  ";
                const std::string optionText =
                    std::string(prefix) +
                    m_activeDialogue->choices[static_cast<std::size_t>(index)].playerText;

                const std::vector<std::string> wrappedOption =
                    PaginateText(optionText.c_str(), DialogueChoiceFontSize, DialogueTextWidth, 2);

                if (!wrappedOption.empty())
                {
                    DrawText(wrappedOption.front().c_str(),
                             DialogueTextLeft,
                             optionsY + index * 24,
                             DialogueChoiceFontSize,
                             (index == m_selectedChoiceIndex) ? YELLOW : WHITE);
                }
            }

            DrawText("W/S lub strzalki = wybor, E/Enter = potwierdz, Esc = zamknij",
                     DialogueTextLeft,
                     Config::ScreenHeight - 95,
                     20,
                     LIGHTGRAY);
        }
    }

    EndDrawing();
}