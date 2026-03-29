#include "battle/battleController.hpp"

#include <raylib.h>

#include "config.hpp"

#include <algorithm>

bool BattleController::IsActive() const
{
    return m_isActive;
}

BattleResult BattleController::GetResult() const
{
    return m_result;
}

void BattleController::Reset()
{
    m_isActive = false;
    m_playerStats = nullptr;
    m_enemyData = nullptr;
    m_enemyCurrentHp = 0;
    m_turn = Turn::Player;
    m_selectedOption = MenuOption::Attack;
    m_result = BattleResult::None;
}

void BattleController::StartBattle(PlayerStats* playerStats, const EnemyData* enemyData)
{
    Reset();

    if (playerStats == nullptr || enemyData == nullptr)
    {
        return;
    }

    m_isActive = true;
    m_playerStats = playerStats;
    m_enemyData = enemyData;
    m_enemyCurrentHp = enemyData->maxHp;
    m_turn = Turn::Player;
}

int BattleController::CalculateDamage(int attack, int defense) const
{
    return std::max(1, attack - defense);
}

void BattleController::HandlePlayerTurn()
{
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
    {
        m_selectedOption = MenuOption::Attack;
    }

    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
    {
        m_selectedOption = MenuOption::Escape;
    }

    if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        if (m_selectedOption == MenuOption::Attack)
        {
            const int damage = CalculateDamage(m_playerStats->attack, m_enemyData->defense);
            m_enemyCurrentHp -= damage;

            if (m_enemyCurrentHp <= 0)
            {
                m_enemyCurrentHp = 0;
                m_result = BattleResult::Victory;
                m_turn = Turn::Finished;
                return;
            }

            m_turn = Turn::Enemy;
            return;
        }

        if (m_selectedOption == MenuOption::Escape)
        {
            m_result = BattleResult::Escape;
            m_turn = Turn::Finished;
        }
    }
}

void BattleController::HandleEnemyTurn()
{
    if (m_playerStats == nullptr || m_enemyData == nullptr)
    {
        return;
    }

    const int damage = CalculateDamage(m_enemyData->attack, m_playerStats->defense);
    m_playerStats->currentHp -= damage;

    if (m_playerStats->currentHp <= 0)
    {
        m_playerStats->currentHp = 0;
        m_result = BattleResult::Defeat;
        m_turn = Turn::Finished;
        return;
    }

    m_turn = Turn::Player;
}

void BattleController::Update()
{
    if (!m_isActive)
    {
        return;
    }

    if (m_turn == Turn::Player)
    {
        HandlePlayerTurn();
        return;
    }

    if (m_turn == Turn::Enemy)
    {
        HandleEnemyTurn();
        return;
    }

    if (m_turn == Turn::Finished)
    {
        if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            m_isActive = false;
        }
    }
}

void BattleController::Draw() const
{
    if (!m_isActive || m_playerStats == nullptr || m_enemyData == nullptr)
    {
        return;
    }

    DrawRectangle(0, 0, Config::ScreenWidth, Config::ScreenHeight, Fade(BLACK, 0.80f));

    DrawText("WALKA", 40, 30, 34, YELLOW);

    DrawRectangle(40, 90, 360, 120, Fade(DARKGRAY, 0.7f));
    DrawRectangleLines(40, 90, 360, 120, WHITE);
    DrawText("Gracz", 60, 105, 28, WHITE);
    DrawText(TextFormat("HP: %d / %d", m_playerStats->currentHp, m_playerStats->maxHp), 60, 145, 24, GREEN);

    DrawRectangle(560, 90, 360, 120, Fade(DARKGRAY, 0.7f));
    DrawRectangleLines(560, 90, 360, 120, WHITE);
    DrawText(m_enemyData->displayName, 580, 105, 28, WHITE);
    DrawText(TextFormat("HP: %d / %d", m_enemyCurrentHp, m_enemyData->maxHp), 580, 145, 24, RED);

    DrawRectangle(40, 430, 880, 150, Fade(DARKGRAY, 0.75f));
    DrawRectangleLines(40, 430, 880, 150, WHITE);

    if (m_turn == Turn::Player)
    {
        DrawText("Twoja tura", 60, 450, 26, YELLOW);

        DrawText(m_selectedOption == MenuOption::Attack ? "> Atak" : "  Atak", 60, 490, 24, WHITE);
        DrawText(m_selectedOption == MenuOption::Escape ? "> Ucieczka" : "  Ucieczka", 60, 520, 24, WHITE);
    }
    else if (m_turn == Turn::Enemy)
    {
        DrawText("Tura przeciwnika...", 60, 480, 26, WHITE);
    }
    else
    {
        if (m_result == BattleResult::Victory)
        {
            DrawText("Wygrales walke!", 60, 470, 28, GREEN);
        }
        else if (m_result == BattleResult::Defeat)
        {
            DrawText("Przegrales walke...", 60, 470, 28, RED);
        }
        else if (m_result == BattleResult::Escape)
        {
            DrawText("Uciekles z walki.", 60, 470, 28, YELLOW);
        }

        DrawText("Nacisnij E / Enter / Spacje", 60, 510, 22, LIGHTGRAY);
    }
}