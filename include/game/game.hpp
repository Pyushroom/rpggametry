#pragma once

#include "dialogue/dialogueController.hpp"
#include "player/Player.hpp"
#include "world/World.hpp"

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

    SceneCoord m_currentCoord{0, 0};
    float m_transitionCooldown{0.0f};
};