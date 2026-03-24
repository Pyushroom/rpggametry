#pragma once

#include <vector>

struct DialogueChoice
{
    const char* playerText{};
    std::vector<const char*> npcResponsePages{};
};

struct DialogueData
{
    const char* speakerName{};
    std::vector<const char*> openingPages{};
    std::vector<DialogueChoice> choices{};
};