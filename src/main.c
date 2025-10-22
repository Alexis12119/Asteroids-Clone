#include "asteroid.h"
#include "bullet.h"
#include "constants.h"
#include "leaderboard.h"
#include "menu.h"
#include "player.h"
#include "raylib.h"
#include "settings.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

// Audio assets
Sound shootSound;
Sound explosionSound;
Music backgroundMusic;
Music gameOverMusic;

Bullet bullets[MAX_BULLETS];
int bulletCount = 0; // Current bullet count
int currentDifficulty = DIFFICULTY_NORMAL; // Current difficulty level
   Settings settings; // Global settings for audio access

   Camera2D camera = {0}; // Camera for responsive scaling

   int GetBulletLimit() {
  switch (currentDifficulty) {
    case DIFFICULTY_EASY: return BULLET_LIMIT_EASY;
    case DIFFICULTY_NORMAL: return BULLET_LIMIT_NORMAL;
    case DIFFICULTY_HARD: return BULLET_LIMIT_HARD;
    default: return BULLET_LIMIT_NORMAL;
  }
}

void ResetGame(Player *player, Asteroid asteroids[], int *lives,
                long long *score, int difficulty) {
  *lives = 3;
  *score = 0;
  InitPlayer(player);
  InitAsteroids(asteroids);
  InitBullets(bullets);

  // Set initial bullet count based on difficulty
  currentDifficulty = difficulty;
  bulletCount = GetBulletLimit();
}

int main(void) {
  srand(time(NULL)); // Initialize random seed once

   InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Geodroid");
   SetWindowState(FLAG_WINDOW_RESIZABLE);
   SetTargetFPS(FPS);

  // Initialize systems
  MenuData menu;
  InitMenu(&menu);

   Leaderboard leaderboard;
   InitLeaderboard(&leaderboard);

   InitSettings(&settings);

   // Sync menu difficulty with settings
   menu.difficulty = settings.difficulty;

    // Initialize audio
    InitAudioDevice();
    shootSound = LoadSound("assets/laser.wav");
    explosionSound = LoadSound("assets/explosion.wav");
    backgroundMusic = LoadMusicStream("assets/spaced.wav");
    gameOverMusic = LoadMusicStream("assets/Icy Game Over.mp3");

  // Game objects
  Player player;
  Asteroid asteroids[MAX_ASTEROIDS] = {0};
  int lives = 3;
  long long score = 0;
  long long finalScore = 0; // For game over screen

  // Respawn invincibility timer
  float invincibilityTimer = 0.0f;

  // Initialize game objects for menu display
  InitPlayer(&player);
  InitAsteroids(asteroids);
  InitBullets(bullets);

   while (!WindowShouldClose()) {
     // Handle fullscreen toggle
     if (IsKeyPressed(KEY_F11)) {
       ToggleFullscreen();
     }

     // Handle menu navigation
     if (menu.currentState == GAME_PLAYING &&
         IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
       Vector2 mousePos = GetMousePosition();
       // Convert to virtual coordinates for scaled UI
       float zoom = fminf((float)GetScreenWidth() / SCREEN_WIDTH, (float)GetScreenHeight() / SCREEN_HEIGHT);
       Vector2 offset = {(GetScreenWidth() - SCREEN_WIDTH * zoom) / 2.0f, (GetScreenHeight() - SCREEN_HEIGHT * zoom) / 2.0f};
       Vector2 virtualMouse = {(mousePos.x - offset.x) / zoom, (mousePos.y - offset.y) / zoom};
       Rectangle pauseButton = {SCREEN_WIDTH - 100, 10, 90, 35};
       if (CheckCollisionPointRec(virtualMouse, pauseButton)) {
         menu.currentState = GAME_PAUSED;
       }
     }

    // Update based on current state
    switch (menu.currentState) {
    case GAME_MENU:
       UpdateMenu(&menu);
       UpdateAsteroids(asteroids);
       break;

    case GAME_PLAYING:
      // Update game logic
      UpdatePlayer(&player);

      // Check if out of bullets
      if (bulletCount <= 0) {
        finalScore = score;
        menu.currentState = GAME_OVER;
      }

       UpdateAsteroids(asteroids);
       UpdateBullets(bullets);

       // Check collisions
      // Bullet-Asteroid collisions
      for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
          for (int j = 0; j < MAX_ASTEROIDS; j++) {
            if (asteroids[j].active) {
              float dx = bullets[i].position.x - asteroids[j].position.x;
              float dy = bullets[i].position.y - asteroids[j].position.y;
              float distance = sqrtf(dx * dx + dy * dy);
                if (distance < asteroids[j].radius) {
                  // Hit!
                  bullets[i].active = false;
                  SplitAsteroid(asteroids, j);
                  score += (asteroids[j].size == ASTEROID_LARGE)    ? 20LL
                           : (asteroids[j].size == ASTEROID_MEDIUM) ? 50LL
                                                                    : 100LL;
                  // Increment bullet count on hit, but don't exceed limit
                  if (bulletCount < GetBulletLimit()) {
                    bulletCount++;
                  }
                   // Play explosion sound
                   if (settings.soundEnabled) PlaySound(explosionSound);
                  break;
                }
            }
          }
        }
      }

      // Player-Asteroid collisions
      bool playerHit = false;

      // Only check collisions if not invincible
      if (invincibilityTimer <= 0.0f) {
        for (int i = 0; i < MAX_ASTEROIDS; i++) {
          if (asteroids[i].active) {
            float dx = player.position.x - asteroids[i].position.x;
            float dy = player.position.y - asteroids[i].position.y;
            float distance = sqrtf(dx * dx + dy * dy);
            if (distance < player.radius + asteroids[i].radius) {
              playerHit = true;
              break;
            }
          }
        }
      }

      // Update invincibility timer
      if (invincibilityTimer > 0.0f) {
        invincibilityTimer -= GetFrameTime();
      }

       if (playerHit) {
         lives--;
         lives = (lives < 0) ? 0 : lives; // Prevent negative lives
         if (settings.soundEnabled) PlaySound(explosionSound);
         if (lives <= 0) {
           finalScore = score;
           menu.currentState = GAME_OVER;
         } else {
          // Reset player position to a safe location
          bool safePosition = false;
          Vector2 newPos;
          int attempts = 0;
          while (!safePosition && attempts < 100) { // Increased attempts
            // Try positions around the center with wider range
            float angle = (float)(rand() % 360) * DEG2RAD;
            float distance =
                80.0f + (float)(rand() % 120); // 80-200 pixels from center
            newPos.x = SCREEN_WIDTH / 2.0f + cosf(angle) * distance;
            newPos.y = SCREEN_HEIGHT / 2.0f + sinf(angle) * distance;

            // Wrap around screen edges
            if (newPos.x < 0)
              newPos.x += SCREEN_WIDTH;
            if (newPos.x >= SCREEN_WIDTH)
              newPos.x -= SCREEN_WIDTH;
            if (newPos.y < 0)
              newPos.y += SCREEN_HEIGHT;
            if (newPos.y >= SCREEN_HEIGHT)
              newPos.y -= SCREEN_HEIGHT;

            // Check if safe from asteroids with larger margin
            safePosition = true;
            for (int j = 0; j < MAX_ASTEROIDS; j++) {
              if (asteroids[j].active) {
                float dx = newPos.x - asteroids[j].position.x;
                float dy = newPos.y - asteroids[j].position.y;
                float dist = sqrtf(dx * dx + dy * dy);
                // Larger safety margin and account for asteroid movement
                if (dist < player.radius + asteroids[j].radius + 50.0f) {
                  safePosition = false;
                  break;
                }
              }
            }
            attempts++;
          }

          if (safePosition) {
            player.position = newPos;
          } else {
            // More sophisticated fallback - find the safest position
            Vector2 bestPos =
                (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
            float bestDistance = 0.0f;

            // Check multiple positions and pick the one farthest from asteroids
            for (int attempt = 0; attempt < 36;
                 attempt++) { // Check every 10 degrees
              float angle = (attempt * 10.0f) * DEG2RAD;
              Vector2 testPos =
                  (Vector2){SCREEN_WIDTH / 2.0f + cosf(angle) * 150.0f,
                            SCREEN_HEIGHT / 2.0f + sinf(angle) * 150.0f};

              // Wrap position
              if (testPos.x < 0)
                testPos.x += SCREEN_WIDTH;
              if (testPos.x >= SCREEN_WIDTH)
                testPos.x -= SCREEN_WIDTH;
              if (testPos.y < 0)
                testPos.y += SCREEN_HEIGHT;
              if (testPos.y >= SCREEN_HEIGHT)
                testPos.y -= SCREEN_HEIGHT;

              // Find minimum distance to any asteroid
              float minDist = 9999.0f;
              for (int j = 0; j < MAX_ASTEROIDS; j++) {
                if (asteroids[j].active) {
                  float dx = testPos.x - asteroids[j].position.x;
                  float dy = testPos.y - asteroids[j].position.y;
                  float dist = sqrtf(dx * dx + dy * dy) - asteroids[j].radius;
                  if (dist < minDist)
                    minDist = dist;
                }
              }

              if (minDist > bestDistance) {
                bestDistance = minDist;
                bestPos = testPos;
              }
            }

            player.position = bestPos;
          }
          player.velocity = (Vector2){0, 0};
          // Give player temporary invincibility after respawn
          invincibilityTimer = 2.0f; // 2 seconds of invincibility
        }
      }
      break;

    case GAME_PAUSED:
      UpdatePauseMenu(&menu);
      break;

    case GAME_OVER:
      UpdateGameOverScreen(&menu);
      break;

     case GAME_LEADERBOARD:
       UpdateLeaderboard();
       UpdateAsteroids(asteroids);
       // Handle back button click
       if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
         Vector2 mousePos = GetMousePosition();
         // Convert to virtual coordinates for scaled UI
         float zoom = fminf((float)GetScreenWidth() / SCREEN_WIDTH, (float)GetScreenHeight() / SCREEN_HEIGHT);
         Vector2 offset = {(GetScreenWidth() - SCREEN_WIDTH * zoom) / 2.0f, (GetScreenHeight() - SCREEN_HEIGHT * zoom) / 2.0f};
         Vector2 virtualMouse = {(mousePos.x - offset.x) / zoom, (mousePos.y - offset.y) / zoom};
         Rectangle backButton = {SCREEN_WIDTH - 120, SCREEN_HEIGHT - 50, 100,
                                 30};
         if (CheckCollisionPointRec(virtualMouse, backButton)) {
           menu.currentState = GAME_MENU;
         }
       }
       break;

     case GAME_SETTINGS:
       UpdateSettings(&settings);
       UpdateAsteroids(asteroids);
       // Handle back button click
       if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
         Vector2 mousePos = GetMousePosition();
         // Convert to virtual coordinates for scaled UI
         float zoom = fminf((float)GetScreenWidth() / SCREEN_WIDTH, (float)GetScreenHeight() / SCREEN_HEIGHT);
         Vector2 offset = {(GetScreenWidth() - SCREEN_WIDTH * zoom) / 2.0f, (GetScreenHeight() - SCREEN_HEIGHT * zoom) / 2.0f};
         Vector2 virtualMouse = {(mousePos.x - offset.x) / zoom, (mousePos.y - offset.y) / zoom};
         Rectangle backButton = {SCREEN_WIDTH - 120, SCREEN_HEIGHT - 50, 100,
                                 30};
         if (CheckCollisionPointRec(virtualMouse, backButton)) {
           menu.currentState = GAME_MENU;
         }
       }
       break;

    case GAME_ENTER_NAME:
       UpdateEnterNameScreen(&menu);
       UpdateAsteroids(asteroids);
       break;
     }

     // Play music
     if (menu.currentState == GAME_OVER && settings.musicEnabled) {
       if (!IsMusicStreamPlaying(gameOverMusic)) {
         StopMusicStream(backgroundMusic);
         PlayMusicStream(gameOverMusic);
       }
     } else if (settings.musicEnabled) {
       StopMusicStream(gameOverMusic);
       if (!IsMusicStreamPlaying(backgroundMusic)) {
         PlayMusicStream(backgroundMusic);
       }
     } else {
       StopMusicStream(backgroundMusic);
       StopMusicStream(gameOverMusic);
     }

     // Update music streams
     UpdateMusicStream(backgroundMusic);
     UpdateMusicStream(gameOverMusic);

     // Handle special transitions
    if (menu.currentState == GAME_OVER && IsKeyPressed(KEY_ENTER)) {
      menu.currentState = GAME_ENTER_NAME;
    }

    // Save score when transitioning to leaderboard from enter name
    static GameState prevState = GAME_MENU;
    if (menu.currentState == GAME_LEADERBOARD && prevState == GAME_ENTER_NAME) {
      AddScore(&leaderboard, menu.playerName, finalScore);
    }
    prevState = menu.currentState;

    // Handle game transitions
    static GameState previousState = GAME_MENU;
    if (menu.currentState == GAME_PLAYING && previousState != GAME_PLAYING) {
      // Just transitioned to playing - reset game if coming from menu or restart
      ResetGame(&player, asteroids, &lives, &score, menu.difficulty);
    }
    previousState = menu.currentState;

    // Handle exit
    static bool exitRequested = false;
    if (menu.currentState == GAME_MENU && IsKeyPressed(KEY_ENTER) &&
        menu.selectedMenuItem == MENU_ITEM_EXIT) {
      exitRequested = true;
    }
    if (exitRequested) {
      break; // Exit game
    }

     // Set up camera for responsive scaling
     camera.zoom = fminf((float)GetScreenWidth() / SCREEN_WIDTH, (float)GetScreenHeight() / SCREEN_HEIGHT);
     camera.offset.x = (GetScreenWidth() - SCREEN_WIDTH * camera.zoom) / 2.0f;
     camera.offset.y = (GetScreenHeight() - SCREEN_HEIGHT * camera.zoom) / 2.0f;

     // Draw
     BeginDrawing();
     ClearBackground(BLACK);
     BeginMode2D(camera);

    switch (menu.currentState) {
    case GAME_MENU:
       DrawAsteroids(asteroids);
       DrawMenu(&menu);
       break;

    case GAME_PLAYING:
      // Draw player with invincibility effect
      if (invincibilityTimer > 0.0f &&
          (int)(invincibilityTimer * 10) % 2 == 0) {
        // Flash effect during invincibility
        DrawPlayer(&player);
      } else if (invincibilityTimer <= 0.0f) {
        DrawPlayer(&player);
      }

      DrawAsteroids(asteroids);
      DrawBullets(bullets);

       // Draw UI
       DrawText(TextFormat("Lives: %d", lives), 10, 10, 20, WHITE);
       DrawText(TextFormat("Score: %lld", score), 10, 40, 20, WHITE);
       DrawText(TextFormat("Bullets: %d", bulletCount), 10, 70, 20, WHITE);
       DrawText(TextFormat("Bullets: %d", bulletCount), 10, 70, 20, WHITE);

      // Draw pause button
      Rectangle pauseButton = {SCREEN_WIDTH - 100, 10, 90, 35};
      DrawRectangleRec(pauseButton, DARKGRAY);
      DrawRectangleLinesEx(pauseButton, 2, WHITE);
      DrawText("PAUSE",
               pauseButton.x +
                   (pauseButton.width - MeasureText("PAUSE", 18)) / 2,
               pauseButton.y + (pauseButton.height - 18) / 2, 18, WHITE);
      break;

    case GAME_PAUSED:
      // Draw game in background
      DrawPlayer(&player);
      DrawAsteroids(asteroids);
      DrawBullets(bullets);
      DrawText(TextFormat("Lives: %d", lives), 10, 10, 20, WHITE);
      DrawText(TextFormat("Score: %lld", score), 10, 40, 20, WHITE);

      DrawPauseMenu(&menu);
      break;

    case GAME_OVER:
      DrawGameOverScreen(finalScore, &menu);
      break;

    case GAME_LEADERBOARD:
       DrawAsteroids(asteroids);
       DrawLeaderboard(&leaderboard);
       break;

    case GAME_SETTINGS:
       DrawAsteroids(asteroids);
       DrawSettings(&settings);
       break;

    case GAME_ENTER_NAME:
       DrawAsteroids(asteroids);
       DrawEnterNameScreen(finalScore, &menu);
       break;
     }

     EndMode2D();
     EndDrawing();
    }

    // Cleanup audio
    UnloadSound(shootSound);
    UnloadSound(explosionSound);
    UnloadMusicStream(backgroundMusic);
    UnloadMusicStream(gameOverMusic);
    CloseAudioDevice();

   CloseWindow();
   return 0;
}
