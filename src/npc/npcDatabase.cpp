#include "npc/NpcDatabase.hpp"

#include "dialogue/DialogueDatabase.hpp"

namespace
{
const NpcData travelerNpc{
    "traveler_01",
    "Eldar",
    "Wedrowiec",
    VIOLET,
    "Nacisnij E, aby porozmawiac",
    {
        NpcDialogueEntry{
            {},
            {},
            &DialogueDatabase::GetTravelerDialogue(),
            nullptr
        }
    }
};

const NpcData guardNpc{
    "guard_01",
    "Borin",
    "Straznik",
    RED,
    "Nacisnij E, aby porozmawiac",
    {
        // Quest zakonczony
        NpcDialogueEntry{
            {"quest_guard_finished"},
            {},
            &DialogueDatabase::GetGuardQuestFinishedDialogue(),
            nullptr
        },

        // Quest gotowy do oddania
        NpcDialogueEntry{
            {"quest_guard_started", "talked_to_cartographer", "talked_to_herbalist"},
            {"quest_guard_finished"},
            &DialogueDatabase::GetGuardQuestCompleteDialogue(),
            "quest_guard_finished"
        },

        // Quest juz trwa
        NpcDialogueEntry{
            {"quest_guard_started"},
            {"talked_to_cartographer", "talked_to_herbalist", "quest_guard_finished"},
            &DialogueDatabase::GetGuardQuestInProgressDialogue(),
            nullptr
        },

        // Pierwsza rozmowa - start questa
        NpcDialogueEntry{
            {},
            {"quest_guard_started", "quest_guard_finished"},
            &DialogueDatabase::GetGuardQuestStartDialogue(),
            "quest_guard_started"
        },

        // Fallback
        NpcDialogueEntry{
            {},
            {},
            &DialogueDatabase::GetGuardAfterDialogue(),
            nullptr
        }
    }
};

const NpcData ladderGuideNpc{
    "worker_01",
    "Tomas",
    "Robotnik",
    ORANGE,
    "Nacisnij E, aby porozmawiac",
    {
        NpcDialogueEntry{
            {},
            {},
            &DialogueDatabase::GetLadderGuideDialogue(),
            nullptr
        }
    }
};

const NpcData hillWatcherNpc{
    "watcher_01",
    "Mira",
    "Obserwator",
    PURPLE,
    "Nacisnij E, aby porozmawiac",
    {
        NpcDialogueEntry{
            {},
            {},
            &DialogueDatabase::GetHillWatcherDialogue(),
            nullptr
        }
    }
};

const NpcData houseOwnerNpc{
    "house_owner_01",
    "Oren",
    "Gospodarz",
    BROWN,
    "Nacisnij E, aby porozmawiac",
    {
        NpcDialogueEntry{
            {},
            {},
            &DialogueDatabase::GetHouseOwnerDialogue(),
            nullptr
        }
    }
};

const NpcData cartographerNpc{
    "cartographer_01",
    "Selim",
    "Kartograf",
    SKYBLUE,
    "Nacisnij E, aby porozmawiac",
    {
        // Po rozmowie w trakcie questa
        NpcDialogueEntry{
            {"quest_guard_started"},
            {"talked_to_cartographer"},
            &DialogueDatabase::GetCartographerQuestDialogue(),
            "talked_to_cartographer"
        },

        // Potem zwykly tekst po queście / po wcześniejszej rozmowie
        NpcDialogueEntry{
            {"talked_to_cartographer"},
            {},
            &DialogueDatabase::GetCartographerAfterQuestDialogue(),
            nullptr
        },

        // Domyslny
        NpcDialogueEntry{
            {},
            {},
            &DialogueDatabase::GetCartographerDialogue(),
            nullptr
        }
    }
};

const NpcData herbalistNpc{
    "herbalist_01",
    "Lira",
    "Zielarka",
    GREEN,
    "Nacisnij E, aby porozmawiac",
    {
        NpcDialogueEntry{
            {"quest_guard_started"},
            {"talked_to_herbalist"},
            &DialogueDatabase::GetHerbalistQuestDialogue(),
            "talked_to_herbalist"
        },

        NpcDialogueEntry{
            {"talked_to_herbalist"},
            {},
            &DialogueDatabase::GetHerbalistAfterQuestDialogue(),
            nullptr
        },

        NpcDialogueEntry{
            {},
            {},
            &DialogueDatabase::GetHerbalistDialogue(),
            nullptr
        }
    }
};
}

namespace NpcDatabase
{
const NpcData& GetTravelerNpc() { return travelerNpc; }
const NpcData& GetGuardNpc() { return guardNpc; }
const NpcData& GetLadderGuideNpc() { return ladderGuideNpc; }
const NpcData& GetHillWatcherNpc() { return hillWatcherNpc; }
const NpcData& GetHouseOwnerNpc() { return houseOwnerNpc; }
const NpcData& GetCartographerNpc() { return cartographerNpc; }
const NpcData& GetHerbalistNpc() { return herbalistNpc; }

static bool MatchesEntry(const NpcDialogueEntry& entry, const GameState& gameState)
{
    for (const char* flag : entry.requiredFlags)
    {
        if (flag == nullptr)
        {
            continue;
        }

        if (!gameState.HasFlag(flag))
        {
            return false;
        }
    }

    for (const char* flag : entry.forbiddenFlags)
    {
        if (flag == nullptr)
        {
            continue;
        }

        if (gameState.HasFlag(flag))
        {
            return false;
        }
    }

    return true;
}

const DialogueData* ResolveDialogue(const NpcData& npcData, const GameState& gameState)
{
    for (const NpcDialogueEntry& entry : npcData.dialogueEntries)
    {
        if (MatchesEntry(entry, gameState))
        {
            return entry.dialogue;
        }
    }

    return nullptr;
}

const char* ResolveSetFlagOnUse(const NpcData& npcData, const GameState& gameState)
{
    for (const NpcDialogueEntry& entry : npcData.dialogueEntries)
    {
        if (MatchesEntry(entry, gameState))
        {
            return entry.setFlagOnUse;
        }
    }

    return nullptr;
}
}