#pragma once

#include "battle/battleTypes.hpp"
#include "enemy/enemyData.hpp"
#include "player/playerStats.hpp"

class BattleController
{
public:
    BattleController() = default;

    void StartBattle(PlayerStats* playerStats, const EnemyData* enemyData);
    void Update();
    void Draw() const;
    void Reset();

    [[nodiscard]] bool IsActive() const;
    [[nodiscard]] BattleResult GetResult() const;

private:
    enum class Turn
    {
        Player,
        Enemy,
        Finished
    };

    enum class MenuOption
    {
        Attack,
        Escape
    };

private:
    void HandlePlayerTurn();
    void HandleEnemyTurn();
    int CalculateDamage(int attack, int defense) const;

private:
    bool m_isActive{false};
    PlayerStats* m_playerStats{nullptr};
    const EnemyData* m_enemyData{nullptr};

    int m_enemyCurrentHp{0};
    Turn m_turn{Turn::Player};
    MenuOption m_selectedOption{MenuOption::Attack};
    BattleResult m_result{BattleResult::None};
};