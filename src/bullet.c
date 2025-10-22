#include "bullet.h"
#include "raylib.h"
#include "constants.h"
#include <math.h>

void InitBullets(Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

void UpdateBullets(Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].position.x += bullets[i].velocity.x * GetFrameTime();
            bullets[i].position.y += bullets[i].velocity.y * GetFrameTime();
            bullets[i].lifetime -= GetFrameTime();

            // Deactivate if lifetime expired or off-screen
            if (bullets[i].lifetime < 0.016f ||  // One frame threshold for floating point
                bullets[i].position.x < -BULLET_RADIUS || bullets[i].position.x > SCREEN_WIDTH + BULLET_RADIUS ||
                bullets[i].position.y < -BULLET_RADIUS || bullets[i].position.y > SCREEN_HEIGHT + BULLET_RADIUS) {
                bullets[i].active = false;
            }
        }
    }
}

void DrawBullets(Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            DrawCircleV(bullets[i].position, BULLET_RADIUS, WHITE);
        }
    }
}

bool FireBullet(Bullet bullets[], Vector2 position, float rotation, int *bulletCount) {
    if (*bulletCount <= 0) {
        return false; // No bullets left
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].position = position;
            bullets[i].velocity = (Vector2){cosf(rotation) * BULLET_SPEED, sinf(rotation) * BULLET_SPEED};
            bullets[i].lifetime = BULLET_LIFETIME;
            bullets[i].active = true;
            (*bulletCount)--; // Decrement bullet count
            return true;
        }
    }
    return false; // No available bullet slots
}