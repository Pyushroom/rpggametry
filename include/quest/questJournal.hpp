#pragma once

#include "game/gameState.hpp"
#include "quest/questData.hpp"

class QuestJournal
{
public:
    QuestJournal() = default;

    void Update(const GameState& gameState);
    void Draw(const GameState& gameState) const;

    [[nodiscard]] bool IsOpen() const;
    [[nodiscard]] const QuestData* GetTrackedQuest(const GameState& gameState) const;

private:
    enum class Tab
    {
        Active,
        Completed
    };

private:
    void Toggle();
    void SwitchToNextTab();
    void SwitchToPreviousTab();
    void ClampSelection(const GameState& gameState);

private:
    bool m_isOpen{false};
    Tab m_currentTab{Tab::Active};

    int m_activeSelectionIndex{0};
    int m_completedSelectionIndex{0};

    const char* m_trackedQuestId{nullptr};
};