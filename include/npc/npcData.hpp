#pragma once

#include <raylib.h>

#include "dialogue/dialogue.hpp"

#include <vector>

struct NpcDialogueEntry
{
    std::vector<const char*> requiredFlags{};
    std::vector<const char*> forbiddenFlags{};
    const DialogueData* dialogue{};
    const char* setFlagOnUse{};
};

struct NpcData
{
    const char* id{};
    const char* displayName{};
    const char* title{};
    Color color{};
    const char* promptText{};
    std::vector<NpcDialogueEntry> dialogueEntries{};
};