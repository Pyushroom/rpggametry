#pragma once

#include "dialogue/Dialogue.hpp"
namespace DialogueDatabase
{
const DialogueData& GetTravelerDialogue();
const DialogueData& GetGuardDialogue();
const DialogueData& GetGuardAfterDialogue();

const DialogueData& GetGuardQuestStartDialogue();
const DialogueData& GetGuardQuestInProgressDialogue();
const DialogueData& GetGuardQuestCompleteDialogue();
const DialogueData& GetGuardQuestFinishedDialogue();

const DialogueData& GetLadderGuideDialogue();
const DialogueData& GetHillWatcherDialogue();
const DialogueData& GetHouseOwnerDialogue();

const DialogueData& GetCartographerDialogue();
const DialogueData& GetCartographerQuestDialogue();
const DialogueData& GetCartographerAfterQuestDialogue();

const DialogueData& GetHerbalistDialogue();
const DialogueData& GetHerbalistQuestDialogue();
const DialogueData& GetHerbalistAfterQuestDialogue();


const DialogueData& GetHillWatcherQuestDialogue();
const DialogueData& GetHillWatcherAfterQuestDialogue();

const DialogueData& GetTravelerQuestFinishedDialogue();
const DialogueData& GetTravelerQuestCompleteDialogue();
const DialogueData& GetTravelerQuestInProgressDialogue();
const DialogueData& GetTravelerQuestStartDialogue();

}