#include "enemy/enemyDatabase.hpp"

namespace
{
    const EnemyData slime{
        "slime_01",
        "Slime",
        BROWN,
        20,
        5,
        1,
        10
    };

    const EnemyData wolf{
        "wolf_01",
        "Wolf",
        GRAY,
        28,
        7,
        2,
        15
    };

    const EnemyData goblin{
        "goblin_01",
        "Goblin",
        GREEN,
        40,
        8,
        3,
        20
    };
}


namespace EnemyDatabase
{
    const EnemyData& GetSlime()
    {
        return slime;
    }

    const EnemyData& GetWolf()
    {
        return wolf;
    }

    const EnemyData& GetGoblin()
    {
        return goblin;
    }
}