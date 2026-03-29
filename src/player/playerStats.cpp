#include "player/playerStats.hpp"

void PlayerStats::AddExperience(int amount)
{
    experience += amount;

    while (experience >= expToNextLevel)
    {
        experience -= expToNextLevel;
        level++;

        // skalowanie (na start proste)
        maxHp += 5;
        attack += 2;
        defense += 1;

        currentHp = maxHp;

        expToNextLevel = static_cast<int>(expToNextLevel * 1.5f);
    }
}