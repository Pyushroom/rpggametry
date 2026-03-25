#pragma once

#include <vector>  

struct QuestObjective{
    const char* description{};
    const char* completionFlag{};
};

struct QuestData{
    const char* id{};
    const char* title{};
    const char* startFlag{};
    const char* completionFlag{};
    std::vector<QuestObjective> objectives{};

};