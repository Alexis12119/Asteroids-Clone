#include "asteroid.h"
#include "raylib.h"
#include "constants.h"
#include "settings.h"
#include <math.h>
#include <stdlib.h>

extern int currentDifficulty;

void InitAsteroids(Asteroid asteroids[]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        asteroids[i].active = false;
    }
    // Spawn initial asteroids away from player
    Vector2 playerPos = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    for (int i = 0; i < 4; i++) {
        Vector2 spawnPos;
        float distance;
        do {
            spawnPos = (Vector2){rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT};
            float dx = spawnPos.x - playerPos.x;
            float dy = spawnPos.y - playerPos.y;
            distance = sqrtf(dx * dx + dy * dy);
        } while (distance < ASTEROID_SPAWN_DISTANCE);
        SpawnAsteroid(asteroids, spawnPos, ASTEROID_LARGE);
    }
}

void UpdateAsteroids(Asteroid asteroids[]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            asteroids[i].position.x += asteroids[i].velocity.x * GetFrameTime();
            asteroids[i].position.y += asteroids[i].velocity.y * GetFrameTime();
            asteroids[i].rotation += asteroids[i].rotationSpeed * GetFrameTime();

            // Wrap around screen
            if (asteroids[i].position.x < -asteroids[i].radius) asteroids[i].position.x = 800 + asteroids[i].radius;
            if (asteroids[i].position.x > 800 + asteroids[i].radius) asteroids[i].position.x = -asteroids[i].radius;
            if (asteroids[i].position.y < -asteroids[i].radius) asteroids[i].position.y = 600 + asteroids[i].radius;
            if (asteroids[i].position.y > 600 + asteroids[i].radius) asteroids[i].position.y = -asteroids[i].radius;
        }
    }
}

void DrawAsteroids(Asteroid asteroids[]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            DrawPolyLines(asteroids[i].position, asteroids[i].sides, asteroids[i].radius, asteroids[i].rotation, WHITE);
        }
    }
}

void SpawnAsteroid(Asteroid asteroids[], Vector2 position, AsteroidSize size) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) {
            asteroids[i].position = position;
            float angle = (float)(rand() % 360) * DEG2RAD;

            // Set speed based on difficulty
            float speedMin, speedMax;
            switch (currentDifficulty) {
                case DIFFICULTY_EASY:
                    speedMin = ASTEROID_SPEED_MIN_EASY;
                    speedMax = ASTEROID_SPEED_MAX_EASY;
                    break;
                case DIFFICULTY_NORMAL:
                    speedMin = ASTEROID_SPEED_MIN_NORMAL;
                    speedMax = ASTEROID_SPEED_MAX_NORMAL;
                    break;
                case DIFFICULTY_HARD:
                    speedMin = ASTEROID_SPEED_MIN_HARD;
                    speedMax = ASTEROID_SPEED_MAX_HARD;
                    break;
                default:
                    speedMin = ASTEROID_SPEED_MIN_NORMAL;
                    speedMax = ASTEROID_SPEED_MAX_NORMAL;
                    break;
            }
            float speed = speedMin + (float)(rand() % (int)(speedMax - speedMin));

            asteroids[i].velocity = (Vector2){cosf(angle) * speed, sinf(angle) * speed};
            asteroids[i].rotation = 0.0f;
            asteroids[i].rotationSpeed = ASTEROID_ROTATION_SPEED_MIN + (float)(rand() % (int)(ASTEROID_ROTATION_SPEED_MAX - ASTEROID_ROTATION_SPEED_MIN));
            asteroids[i].size = size;
            asteroids[i].sides = 5 + rand() % 4; // 5-8 sides
            switch (size) {
                case ASTEROID_LARGE: asteroids[i].radius = ASTEROID_RADIUS_LARGE; break;
                case ASTEROID_MEDIUM: asteroids[i].radius = ASTEROID_RADIUS_MEDIUM; break;
                case ASTEROID_SMALL: asteroids[i].radius = ASTEROID_RADIUS_SMALL; break;
            }
            asteroids[i].active = true;
            break;
        }
    }
}

void SplitAsteroid(Asteroid asteroids[], int index) {
    AsteroidSize newSize;
    int numSplits = 0;
    switch (asteroids[index].size) {
        case ASTEROID_LARGE:
            newSize = ASTEROID_MEDIUM;
            numSplits = 2;
            break;
        case ASTEROID_MEDIUM:
            newSize = ASTEROID_SMALL;
            numSplits = 2;
            break;
        case ASTEROID_SMALL:
            // No split, just deactivate
            asteroids[index].active = false;
            return;
    }
    for (int i = 0; i < numSplits; i++) {
        // Add random offset to prevent immediate collision
        Vector2 offset = (Vector2){(rand() % 40) - 20, (rand() % 40) - 20};
        Vector2 spawnPos = (Vector2){asteroids[index].position.x + offset.x, asteroids[index].position.y + offset.y};
        SpawnAsteroid(asteroids, spawnPos, newSize);
    }
    asteroids[index].active = false;
}