#pragma once

#include "dialogue/dialogueController.hpp"
#include "game/gameState.hpp"
#include "player/player.hpp"
#include "player/playerStats.hpp"
#include "quest/questJournal.hpp"
#include "world/world.hpp"
#include "battle/battleController.hpp"

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
    GameState m_gameState{};
    BattleController m_battleController{};

    SceneCoord m_currentCoord{0, 0};
    float m_transitionCooldown{0.0f};
};