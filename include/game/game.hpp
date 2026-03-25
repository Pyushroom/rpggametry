#pragma once

#include "dialogue/dialogueController.hpp"
#include "player/player.hpp"
#include "world/world.hpp"
#include "gameState.hpp"
#include "dialogue/dialogueDatabase.hpp"
#include "npc/npcDatabase.hpp"

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
    GameState m_gameState{};

    SceneCoord m_currentCoord{0, 0};
    float m_transitionCooldown{0.0f};
};