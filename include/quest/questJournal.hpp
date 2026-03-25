#pragma once

#include "game/gameState.hpp"

class QuestJournal
{
public:
    QuestJournal() = default;

    void Update(const GameState& gameState);
    void Draw(const GameState& gameState) const;

    [[nodiscard]] bool IsOpen() const;

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

private:
    bool m_isOpen{false};
    Tab m_currentTab{Tab::Active};
};