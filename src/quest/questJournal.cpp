#include "quest/questJournal.hpp"

#include <raylib.h>

#include "config.hpp"
#include "quest/questSystem.hpp"

#include <vector>

namespace
{
constexpr int JournalX = 90;
constexpr int JournalY = 60;
constexpr int JournalWidth = Config::ScreenWidth - 180;
constexpr int JournalHeight = Config::ScreenHeight - 120;
}

bool QuestJournal::IsOpen() const
{
    return m_isOpen;
}

void QuestJournal::Toggle()
{
    m_isOpen = !m_isOpen;
}

void QuestJournal::SwitchToNextTab()
{
    if (m_currentTab == Tab::Active)
    {
        m_currentTab = Tab::Completed;
    }
    else
    {
        m_currentTab = Tab::Active;
    }
}

void QuestJournal::SwitchToPreviousTab()
{
    SwitchToNextTab();
}

void QuestJournal::Update(const GameState& gameState)
{
    (void)gameState;

    if (IsKeyPressed(KEY_J))
    {
        Toggle();
        return;
    }

    if (!m_isOpen)
    {
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        m_isOpen = false;
        return;
    }

    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
    {
        SwitchToPreviousTab();
    }

    if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_TAB))
    {
        SwitchToNextTab();
    }
}

void QuestJournal::Draw(const GameState& gameState) const
{
    if (!m_isOpen)
    {
        return;
    }

    const std::vector<const QuestData*> quests =
        (m_currentTab == Tab::Active)
            ? QuestSystem::GetActiveQuests(gameState)
            : QuestSystem::GetCompletedQuests(gameState);

    DrawRectangle(0, 0, Config::ScreenWidth, Config::ScreenHeight, Fade(BLACK, 0.45f));
    DrawRectangle(JournalX, JournalY, JournalWidth, JournalHeight, Fade(BLACK, 0.90f));
    DrawRectangleLines(JournalX, JournalY, JournalWidth, JournalHeight, WHITE);

    DrawText("Dziennik zadan", JournalX + 24, JournalY + 18, 30, YELLOW);

    const bool activeTabSelected = (m_currentTab == Tab::Active);
    DrawRectangle(JournalX + 24, JournalY + 60, 140, 36, activeTabSelected ? DARKGRAY : Fade(GRAY, 0.3f));
    DrawRectangleLines(JournalX + 24, JournalY + 60, 140, 36, WHITE);
    DrawText("Aktywne", JournalX + 50, JournalY + 69, 20, WHITE);

    const bool completedTabSelected = (m_currentTab == Tab::Completed);
    DrawRectangle(JournalX + 180, JournalY + 60, 160, 36, completedTabSelected ? DARKGRAY : Fade(GRAY, 0.3f));
    DrawRectangleLines(JournalX + 180, JournalY + 60, 160, 36, WHITE);
    DrawText("Ukonczone", JournalX + 206, JournalY + 69, 20, WHITE);

    int y = JournalY + 120;

    if (quests.empty())
    {
        DrawText("Brak zadan w tej zakladce.", JournalX + 24, y, 22, LIGHTGRAY);
    }
    else
    {
        for (const QuestData* quest : quests)
        {
            if (quest == nullptr)
            {
                continue;
            }

            DrawText(quest->title, JournalX + 24, y, 24, YELLOW);
            y += 32;

            for (const QuestObjective& objective : quest->objectives)
            {
                const bool completed = QuestSystem::IsObjectiveCompleted(objective, gameState);

                DrawText(
                    TextFormat("[%c] %s", completed ? 'x' : ' ', objective.description),
                    JournalX + 36,
                    y,
                    20,
                    completed ? GREEN : WHITE
                );

                y += 24;
            }

            y += 18;
        }
    }

    DrawText("J / Esc = zamknij, A/D lub strzalki = zmiana zakladki",
             JournalX + 24,
             JournalY + JournalHeight - 34,
             20,
             LIGHTGRAY);
}