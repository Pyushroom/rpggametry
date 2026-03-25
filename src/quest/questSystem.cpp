#include "quest/questSystem.hpp"
#include "quest/questDatabase.hpp"

namespace QuestSystem
{
bool IsQuestStarted(const QuestData& quest, const GameState& gameState)
{
    if (quest.startFlag == nullptr)
    {
        return false;
    }

    return gameState.HasFlag(quest.startFlag);
}

bool IsQuestCompleted(const QuestData& quest, const GameState& gameState)
{
    if (quest.completionFlag == nullptr)
    {
        return false;
    }

    return gameState.HasFlag(quest.completionFlag);
}

bool IsObjectiveCompleted(const QuestObjective& objective, const GameState& gameState)
{
    if (objective.completionFlag == nullptr)
    {
        return false;
    }

    return gameState.HasFlag(objective.completionFlag);
}

std::vector<const QuestData*> GetActiveQuests(const GameState& gameState)
{
    std::vector<const QuestData*> result;

    for (const QuestData& quest : QuestDatabase::GetAllQuests())
    {
        if (IsQuestStarted(quest, gameState) && !IsQuestCompleted(quest, gameState))
        {
            result.push_back(&quest);
        }
    }

    return result;
}

std::vector<const QuestData*> GetCompletedQuests(const GameState& gameState)
{
    std::vector<const QuestData*> result;

    for (const QuestData& quest : QuestDatabase::GetAllQuests())
    {
        if (IsQuestCompleted(quest, gameState))
        {
            result.push_back(&quest);
        }
    }

    return result;
}

const QuestData* GetLatestActiveQuest(const GameState& gameState)
{
    const std::vector<const QuestData*> activeQuests = GetActiveQuests(gameState);

    if (activeQuests.empty())
    {
        return nullptr;
    }

    return activeQuests.back();
}
}