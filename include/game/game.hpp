#pragma once

#include "battle/battleController.hpp"
#include "dialogue/dialogueController.hpp"
#include "game/gameState.hpp"
#include "player/playerStats.hpp"
#include "player/player.hpp"
#include "quest/questJournal.hpp"
#include "world/world.hpp"

#include <optional>

class Game
{
public:
    Game();

    int Run();

private:
    void Update(float deltaTime);
    void Draw() const;

private:
    World m_world{};
    Player m_player{};
    PlayerStats m_playerStats{};
    DialogueController m_dialogueController{};
    QuestJournal m_questJournal{};
    BattleController m_battleController{};
    GameState m_gameState{};

    SceneCoord m_currentCoord{0, 0};
    float m_transitionCooldown{0.0f};

    std::optional<std::size_t> m_activeEnemyIndex{};
    SceneCoord m_battleSceneCoord{0, 0};
    float m_enemyEncounterCooldown{0.0f};
};