#pragma once

#include "dialogue/dialogueController.hpp"
#include "game/gameState.hpp"
#include "player/player.hpp"
#include "quest/questJournal.hpp"
#include "world/world.hpp"

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
    DialogueController m_dialogueController{};
    QuestJournal m_questJournal{};
    GameState m_gameState{};

    SceneCoord m_currentCoord{0, 0};
    float m_transitionCooldown{0.0f};
};