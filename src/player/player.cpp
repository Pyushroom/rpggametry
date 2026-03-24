#include "player/player.hpp"

#include "config.hpp"

void ClampPlayerToScreen(Player& player)
{
    if (player.rect.x < 0.0f)
    {
        player.rect.x = 0.0f;
    }

    if (player.rect.y < 0.0f)
    {
        player.rect.y = 0.0f;
    }

    if (player.rect.x + player.rect.width > static_cast<float>(Config::ScreenWidth))
    {
        player.rect.x = static_cast<float>(Config::ScreenWidth) - player.rect.width;
    }

    if (player.rect.y + player.rect.height > static_cast<float>(Config::ScreenHeight))
    {
        player.rect.y = static_cast<float>(Config::ScreenHeight) - player.rect.height;
    }
}

void MovePlayer(Player& player, const Scene& scene, float deltaTime)
{
    float moveX = 0.0f;
    float moveY = 0.0f;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        moveX -= player.speed * deltaTime;
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        moveX += player.speed * deltaTime;
    }

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        moveY -= player.speed * deltaTime;
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        moveY += player.speed * deltaTime;
    }

    Rectangle nextX = player.rect;
    nextX.x += moveX;

    if (nextX.x < 0.0f)
    {
        nextX.x = 0.0f;
    }

    if (nextX.x + nextX.width > static_cast<float>(Config::ScreenWidth))
    {
        nextX.x = static_cast<float>(Config::ScreenWidth) - nextX.width;
    }

    if (!CollidesWithBlockingObjects(nextX, scene))
    {
        player.rect.x = nextX.x;
    }

    Rectangle nextY = player.rect;
    nextY.y += moveY;

    if (nextY.y < 0.0f)
    {
        nextY.y = 0.0f;
    }

    if (nextY.y + nextY.height > static_cast<float>(Config::ScreenHeight))
    {
        nextY.y = static_cast<float>(Config::ScreenHeight) - nextY.height;
    }

    if (!CollidesWithBlockingObjects(nextY, scene))
    {
        player.rect.y = nextY.y;
    }
}

void SetPlayerPositionAfterTransition(Player& player, Direction direction)
{
    switch (direction)
    {
        case Direction::Left:
            player.rect.x = static_cast<float>(Config::ScreenWidth) - player.rect.width - Config::PlayerSpawnMargin;
            break;
        case Direction::Right:
            player.rect.x = Config::PlayerSpawnMargin;
            break;
        case Direction::Up:
            player.rect.y = static_cast<float>(Config::ScreenHeight) - player.rect.height - Config::PlayerSpawnMargin;
            break;
        case Direction::Down:
            player.rect.y = Config::PlayerSpawnMargin;
            break;
    }

    ClampPlayerToScreen(player);
}

void SetPlayerPosition(Player& player, Vector2 position)
{
    player.rect.x = position.x;
    player.rect.y = position.y;
    ClampPlayerToScreen(player);
}