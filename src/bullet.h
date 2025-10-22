#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float lifetime;
    bool active;
} Bullet;

#define MAX_BULLETS 50

void InitBullets(Bullet bullets[]);
void UpdateBullets(Bullet bullets[]);
void DrawBullets(Bullet bullets[]);
bool FireBullet(Bullet bullets[], Vector2 position, float rotation, int *bulletCount);

#endif