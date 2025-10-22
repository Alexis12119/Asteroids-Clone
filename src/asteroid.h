#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"

typedef enum {
    ASTEROID_LARGE,
    ASTEROID_MEDIUM,
    ASTEROID_SMALL
} AsteroidSize;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float rotationSpeed;
    AsteroidSize size;
    int sides;
    float radius;
    bool active;
} Asteroid;

#define MAX_ASTEROIDS 100

void InitAsteroids(Asteroid asteroids[]);
void UpdateAsteroids(Asteroid asteroids[]);
void DrawAsteroids(Asteroid asteroids[]);
void SpawnAsteroid(Asteroid asteroids[], Vector2 position, AsteroidSize size);
void SplitAsteroid(Asteroid asteroids[], int index);

#endif