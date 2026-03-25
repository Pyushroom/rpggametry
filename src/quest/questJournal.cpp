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

constexpr int LeftColumnX = JournalX + 24;
constexpr int LeftColumnY = JournalY + 120;
constexpr int LeftColumnWidth = 320;

constexpr int RightColumnX = JournalX + 370;
constexpr int RightColumnY = JournalY + 120;
constexpr int RightColumnWidth = JournalWidth - 394;
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

void QuestJournal::ClampSelection(const GameState& gameState)
{
    const std::vector<const QuestData*> activeQuests = QuestSystem::GetActiveQuests(gameState);
    const std::vector<const QuestData*> completedQuests = QuestSystem::GetCompletedQuests(gameState);

    if (activeQuests.empty())
    {
        m_activeSelectionIndex = 0;
    }
    else
    {
        if (m_activeSelectionIndex < 0)
        {
            m_activeSelectionIndex = 0;
        }
        if (m_activeSelectionIndex >= static_cast<int>(activeQuests.size()))
        {
            m_activeSelectionIndex = static_cast<int>(activeQuests.size()) - 1;
        }
    }

    if (completedQuests.empty())
    {
        m_completedSelectionIndex = 0;
    }
    else
    {
        if (m_completedSelectionIndex < 0)
        {
            m_completedSelectionIndex = 0;
        }
        if (m_completedSelectionIndex >= static_cast<int>(completedQuests.size()))
        {
            m_completedSelectionIndex = static_cast<int>(completedQuests.size()) - 1;
        }
    }
}

const QuestData* QuestJournal::GetTrackedQuest(const GameState& gameState) const
{
    const std::vector<const QuestData*> activeQuests = QuestSystem::GetActiveQuests(gameState);

    if (m_trackedQuestId != nullptr)
    {
        for (const QuestData* quest : activeQuests)
        {
            if (quest != nullptr && quest->id != nullptr && std::string(quest->id) == m_trackedQuestId)
            {
                return quest;
            }
        }
    }

    if (activeQuests.empty())
    {
        return nullptr;
    }

    return activeQuests.back();
}

void QuestJournal::Update(const GameState& gameState)
{
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

    ClampSelection(gameState);

    const std::vector<const QuestData*> activeQuests = QuestSystem::GetActiveQuests(gameState);
    const std::vector<const QuestData*> completedQuests = QuestSystem::GetCompletedQuests(gameState);

    if (m_currentTab == Tab::Active)
    {
        if (!activeQuests.empty())
        {
            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
            {
                --m_activeSelectionIndex;
                if (m_activeSelectionIndex < 0)
                {
                    m_activeSelectionIndex = static_cast<int>(activeQuests.size()) - 1;
                }
            }

            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
            {
                ++m_activeSelectionIndex;
                if (m_activeSelectionIndex >= static_cast<int>(activeQuests.size()))
                {
                    m_activeSelectionIndex = 0;
                }
            }

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
            {
                const QuestData* selectedQuest = activeQuests[static_cast<std::size_t>(m_activeSelectionIndex)];
                if (selectedQuest != nullptr)
                {
                    m_trackedQuestId = selectedQuest->id;
                }
            }
        }
    }
    else
    {
        if (!completedQuests.empty())
        {
            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
            {
                --m_completedSelectionIndex;
                if (m_completedSelectionIndex < 0)
                {
                    m_completedSelectionIndex = static_cast<int>(completedQuests.size()) - 1;
                }
            }

            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
            {
                ++m_completedSelectionIndex;
                if (m_completedSelectionIndex >= static_cast<int>(completedQuests.size()))
                {
                    m_completedSelectionIndex = 0;
                }
            }
        }
    }

    ClampSelection(gameState);
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

    const int selectedIndex =
        (m_currentTab == Tab::Active)
            ? m_activeSelectionIndex
            : m_completedSelectionIndex;

    const QuestData* selectedQuest =
        (!quests.empty() && selectedIndex >= 0 && selectedIndex < static_cast<int>(quests.size()))
            ? quests[static_cast<std::size_t>(selectedIndex)]
            : nullptr;

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

    DrawRectangleLines(JournalX + 350, JournalY + 110, 2, JournalHeight - 150, WHITE);

    if (quests.empty())
    {
        DrawText("Brak zadan w tej zakladce.", LeftColumnX, LeftColumnY, 22, LIGHTGRAY);
    }
    else
    {
        int y = LeftColumnY;

        for (int index = 0; index < static_cast<int>(quests.size()); ++index)
        {
            const QuestData* quest = quests[static_cast<std::size_t>(index)];
            if (quest == nullptr)
            {
                continue;
            }

            const bool selected = (index == selectedIndex);
            const bool tracked =
                (quest->id != nullptr && m_trackedQuestId != nullptr && std::string(quest->id) == m_trackedQuestId);

            DrawRectangle(
                LeftColumnX - 8,
                y - 4,
                LeftColumnWidth - 8,
                30,
                selected ? Fade(DARKGRAY, 0.85f) : Fade(GRAY, 0.15f)
            );

            DrawText(
                tracked ? TextFormat("* %s", quest->title) : quest->title,
                LeftColumnX,
                y,
                22,
                selected ? YELLOW : WHITE
            );

            y += 36;
        }
    }

    if (selectedQuest != nullptr)
    {
        DrawText(selectedQuest->title, RightColumnX, RightColumnY, 26, YELLOW);

        int y = RightColumnY + 38;
        for (const QuestObjective& objective : selectedQuest->objectives)
        {
            const bool completed = QuestSystem::IsObjectiveCompleted(objective, gameState);

            DrawText(
                TextFormat("[%c] %s", completed ? 'x' : ' ', objective.description),
                RightColumnX,
                y,
                20,
                completed ? GREEN : WHITE
            );

            y += 26;
        }

        if (m_currentTab == Tab::Active)
        {
            DrawText("Enter / Spacja = przypnij zadanie", RightColumnX, JournalY + JournalHeight - 60, 20, LIGHTGRAY);
        }
    }

    DrawText("J / Esc = zamknij, A/D lub strzalki = zmiana zakladki, W/S = wybor",
             JournalX + 24,
             JournalY + JournalHeight - 30,
             20,
             LIGHTGRAY);
}