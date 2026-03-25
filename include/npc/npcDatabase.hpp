#pragma once

#include "game/GameState.hpp"
#include "npc/NpcData.hpp"

namespace NpcDatabase
{
const NpcData& GetTravelerNpc();
const NpcData& GetGuardNpc();
const NpcData& GetLadderGuideNpc();
const NpcData& GetHillWatcherNpc();
const NpcData& GetHouseOwnerNpc();
const NpcData& GetCartographerNpc();
const NpcData& GetHerbalistNpc();

const DialogueData* ResolveDialogue(const NpcData& npcData, const GameState& gameState);
const char* ResolveSetFlagOnUse(const NpcData& npcData, const GameState& gameState);
}