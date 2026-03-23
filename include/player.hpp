#pragma once

#include <raylib.h>

#include "scene.hpp"

struct Player
{
    Rectangle rect{};
    float speed{};
};

void MovePlayer(Player& player, const Scene& scene, float deltaTime);
void ClampPlayerToScreen(Player& player);
void SetPlayerPositionAfterTransition(Player& player, Direction direction);
void SetPlayerPosition(Player& player, Vector2 position);