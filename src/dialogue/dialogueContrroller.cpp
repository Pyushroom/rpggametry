#include "dialogue/dialogueController.hpp"

#include <raylib.h>

#include "config.hpp"

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

bool DialogueController::IsActive() const
{
    return m_mode != Mode::Hidden;
}

bool DialogueController::IsHidden() const
{
    return m_mode == Mode::Hidden;
}

void DialogueController::Close()
{
    ResetState();
}

void DialogueController::ResetState()
{
    m_mode = Mode::Hidden;
    m_activeDialogue = nullptr;
    m_currentPages.clear();
    m_currentPageIndex = 0;
    m_selectedChoiceIndex = 0;
    m_selectedChoiceText = nullptr;
}

std::vector<std::string> DialogueController::WrapTextToLines(
    const char* text,
    int fontSize,
    int maxWidth) const
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

        const std::string candidate = currentLine.empty()
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

std::vector<std::string> DialogueController::PaginateText(
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

void DialogueController::SetPagesFromText(
    const char* text,
    int fontSize,
    int maxWidth,
    int maxLinesPerPage)
{
    m_currentPages = PaginateText(text, fontSize, maxWidth, maxLinesPerPage);
    m_currentPageIndex = 0;
}

void DialogueController::StartDialogue(const DialogueData* dialogueData)
{
    ResetState();

    if (dialogueData == nullptr)
    {
        return;
    }

    m_activeDialogue = dialogueData;
    m_selectedChoiceIndex = 0;

    if (!dialogueData->openingPages.empty())
    {
        std::string joinedOpening;
        for (std::size_t index = 0; index < dialogueData->openingPages.size(); ++index)
        {
            if (index > 0)
            {
                joinedOpening += '\n';
            }

            joinedOpening += dialogueData->openingPages[index];
        }

        SetPagesFromText(
            joinedOpening.c_str(),
            DialogueBodyFontSize,
            DialogueTextWidth,
            DialogueMaxLinesPerPage
        );

        m_mode = Mode::OpeningPages;
    }
    else if (!dialogueData->choices.empty())
    {
        m_mode = Mode::ChoiceSelection;
    }
}

void DialogueController::AdvancePage()
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

    if (m_mode == Mode::OpeningPages)
    {
        if (m_activeDialogue != nullptr && !m_activeDialogue->choices.empty())
        {
            m_mode = Mode::ChoiceSelection;
            m_currentPages.clear();
            m_currentPageIndex = 0;
            return;
        }

        Close();
        return;
    }

    if (m_mode == Mode::ResponsePages)
    {
        if (m_activeDialogue != nullptr && !m_activeDialogue->choices.empty())
        {
            m_mode = Mode::ChoiceSelection;
            m_currentPages.clear();
            m_currentPageIndex = 0;
            return;
        }

        Close();
    }
}

void DialogueController::Update()
{
    if (m_mode == Mode::Hidden)
    {
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        Close();
        return;
    }

    if (m_mode == Mode::OpeningPages || m_mode == Mode::ResponsePages)
    {
        if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            AdvancePage();
        }

        return;
    }

    if (m_mode == Mode::ChoiceSelection)
    {
        if (m_activeDialogue == nullptr)
        {
            Close();
            return;
        }

        const int choiceCount = static_cast<int>(m_activeDialogue->choices.size());

        if (choiceCount <= 0)
        {
            Close();
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

            SetPagesFromText(
                joinedResponse.c_str(),
                DialogueBodyFontSize,
                DialogueTextWidth,
                DialogueResponseMaxLinesPerPage
            );

            if (!m_currentPages.empty())
            {
                m_mode = Mode::ResponsePages;
            }
        }
    }
}

void DialogueController::Draw() const
{
    if (m_mode == Mode::Hidden || m_activeDialogue == nullptr)
    {
        return;
    }

    DrawRectangle(DialogueBoxX, DialogueBoxY, DialogueBoxWidth, DialogueBoxHeight, Fade(BLACK, 0.88f));
    DrawRectangleLines(DialogueBoxX, DialogueBoxY, DialogueBoxWidth, DialogueBoxHeight, WHITE);

    DrawText(m_activeDialogue->speakerName, DialogueTextLeft, Config::ScreenHeight - 228, 28, YELLOW);

    if ((m_mode == Mode::OpeningPages || m_mode == Mode::ResponsePages) &&
        !m_currentPages.empty() &&
        m_currentPageIndex >= 0 &&
        m_currentPageIndex < static_cast<int>(m_currentPages.size()))
    {
        if (m_mode == Mode::ResponsePages && m_selectedChoiceText != nullptr)
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
    else if (m_mode == Mode::ChoiceSelection)
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