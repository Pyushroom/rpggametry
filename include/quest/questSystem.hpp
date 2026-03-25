#pragma once

#include "game/gameState.hpp"
#include "quest/questData.hpp"

#include <vector>

namespace QuestSystem
{
bool IsQuestStarted(const QuestData& quest, const GameState& gameState);
bool IsQuestCompleted(const QuestData& quest, const GameState& gameState);
bool IsObjectiveCompleted(const QuestObjective& objective, const GameState& gameState);

std::vector<const QuestData*> GetActiveQuests(const GameState& gameState);
std::vector<const QuestData*> GetCompletedQuests(const GameState& gameState);
const QuestData* GetLatestActiveQuest(const GameState& gameState);
}