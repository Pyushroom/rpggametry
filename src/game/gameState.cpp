#include "game/gameState.hpp"

void GameState::SetFlag(const std::string& flag)
{
    m_flags.insert(flag);
}

bool GameState::HasFlag(const std::string& flag) const
{
    return m_flags.find(flag) != m_flags.end();
}