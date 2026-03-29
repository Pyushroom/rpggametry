#pragma once

#include <raylib.h>

struct EnemyData
{
    const char* id{};
    const char* displayName{};
    Color color{};
    int maxHp{};
    int attack{};
    int defense{};
    int expReward{};
};
