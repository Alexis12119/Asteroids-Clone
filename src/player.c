#include "player.h"
#include "raylib.h"
#include "bullet.h"
#include "constants.h"
#include <math.h>

extern Bullet bullets[MAX_BULLETS];
extern int bulletCount;

void InitPlayer(Player *player) {
    player->position = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    player->velocity = (Vector2){0, 0};
    player->rotation = 0.0f;
    player->radius = PLAYER_RADIUS;
}

void UpdatePlayer(Player *player) {
    // Rotation
    if (IsKeyDown(KEY_LEFT)) player->rotation -= PLAYER_ROTATION_SPEED * GetFrameTime();
    if (IsKeyDown(KEY_RIGHT)) player->rotation += PLAYER_ROTATION_SPEED * GetFrameTime();

    // Thrust
    if (IsKeyDown(KEY_UP)) {
        Vector2 direction = (Vector2){cosf(player->rotation), sinf(player->rotation)};
        player->velocity.x += direction.x * PLAYER_SPEED * GetFrameTime();
        player->velocity.y += direction.y * PLAYER_SPEED * GetFrameTime();
    }

    // Apply friction
    player->velocity.x *= FRICTION;
    player->velocity.y *= FRICTION;

    // Cap velocity
    float speed = sqrtf(player->velocity.x * player->velocity.x + player->velocity.y * player->velocity.y);
    if (speed > PLAYER_MAX_SPEED) {
        player->velocity.x = (player->velocity.x / speed) * PLAYER_MAX_SPEED;
        player->velocity.y = (player->velocity.y / speed) * PLAYER_MAX_SPEED;
    }

    // Update position
    player->position.x += player->velocity.x * GetFrameTime();
    player->position.y += player->velocity.y * GetFrameTime();

    // Wrap around screen
    if (player->position.x < 0) player->position.x = SCREEN_WIDTH;
    if (player->position.x > SCREEN_WIDTH) player->position.x = 0;
    if (player->position.y < 0) player->position.y = SCREEN_HEIGHT;
    if (player->position.y > SCREEN_HEIGHT) player->position.y = 0;

    // Shooting
    if (IsKeyPressed(KEY_SPACE)) {
        FireBullet(bullets, player->position, player->rotation, &bulletCount);
    }
}

void DrawPlayer(Player *player) {
    Vector2 points[3];
    points[0] = (Vector2){player->position.x + cosf(player->rotation) * player->radius,
                          player->position.y + sinf(player->rotation) * player->radius};
    points[1] = (Vector2){player->position.x + cosf(player->rotation + 2.5f) * player->radius * 0.7f,
                          player->position.y + sinf(player->rotation + 2.5f) * player->radius * 0.7f};
    points[2] = (Vector2){player->position.x + cosf(player->rotation - 2.5f) * player->radius * 0.7f,
                          player->position.y + sinf(player->rotation - 2.5f) * player->radius * 0.7f};

    DrawTriangleLines(points[0], points[1], points[2], WHITE);
}