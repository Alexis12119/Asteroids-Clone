#include "menu.h"
#include "constants.h"
#include "settings.h"
#include <string.h>
#include <math.h>

#define MENU_TITLE "GEODROID"
#define MENU_TITLE_SIZE 60
#define MENU_ITEM_SIZE 30
#define MENU_ITEM_SPACING 50

static const char *menuItems[MENU_ITEM_COUNT] = {
    "PLAY GAME",
    "LEADERBOARD",
    "SETTINGS",
    "EXIT"
};

static const char *pauseItems[PAUSE_ITEM_COUNT] = {
    "RESUME",
    "RESTART",
    "MAIN MENU",
    "SETTINGS"
};



void InitMenu(MenuData *menu) {
    menu->currentState = GAME_MENU;
    menu->selectedMenuItem = MENU_ITEM_PLAY;
    menu->selectedPauseItem = PAUSE_ITEM_RESUME;
    menu->soundEnabled = true;
    menu->difficulty = DIFFICULTY_NORMAL;
    strcpy(menu->playerName, "PLAYER");
    menu->nameCharIndex = 0;
}

void UpdateMenu(MenuData *menu) {
    if (IsKeyPressed(KEY_DOWN)) {
        menu->selectedMenuItem = (menu->selectedMenuItem + 1) % MENU_ITEM_COUNT;
    }
    if (IsKeyPressed(KEY_UP)) {
        menu->selectedMenuItem = (menu->selectedMenuItem - 1 + MENU_ITEM_COUNT) % MENU_ITEM_COUNT;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        switch (menu->selectedMenuItem) {
            case MENU_ITEM_PLAY:
                menu->currentState = GAME_PLAYING;
                break;
            case MENU_ITEM_LEADERBOARD:
                menu->currentState = GAME_LEADERBOARD;
                break;
            case MENU_ITEM_SETTINGS:
                menu->currentState = GAME_SETTINGS;
                break;
            case MENU_ITEM_EXIT:
                // Will be handled in main loop
                break;
            default:
                break;
        }
    }
}

void DrawMenu(MenuData *menu) {
    // Draw title with animated effect
    float titleY = SCREEN_HEIGHT / 4.0f + sinf(GetTime() * 2.0f) * 10.0f;
    DrawText(MENU_TITLE, SCREEN_WIDTH / 2 - MeasureText(MENU_TITLE, MENU_TITLE_SIZE) / 2, titleY, MENU_TITLE_SIZE, WHITE);

    // Draw menu items
    for (int i = 0; i < MENU_ITEM_COUNT; i++) {
        Color color = (i == menu->selectedMenuItem) ? YELLOW : WHITE;
        int y = SCREEN_HEIGHT / 2 + i * MENU_ITEM_SPACING;
        DrawText(menuItems[i], SCREEN_WIDTH / 2 - MeasureText(menuItems[i], MENU_ITEM_SIZE) / 2, y, MENU_ITEM_SIZE, color);
    }

    // Draw instructions
    DrawText("Use UP/DOWN arrows to navigate, ENTER to select", 10, SCREEN_HEIGHT - 30, 20, GRAY);
}





void DrawGameOverScreen(long long score, MenuData *menu) {
    DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 50) / 2, SCREEN_HEIGHT / 3, 50, RED);
    DrawText(TextFormat("Final Score: %lld", score), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Final Score: %lld", score), 30) / 2, SCREEN_HEIGHT / 2, 30, WHITE);

    DrawText("Press ENTER to continue", SCREEN_WIDTH / 2 - MeasureText("Press ENTER to continue", 20) / 2, SCREEN_HEIGHT - 50, 20, GRAY);
}

void UpdateGameOverScreen(MenuData *menu) {
    if (IsKeyPressed(KEY_ENTER)) {
        menu->currentState = GAME_ENTER_NAME;
    }
}

void DrawEnterNameScreen(long long score, MenuData *menu) {
    DrawText("ENTER YOUR NAME", SCREEN_WIDTH / 2 - MeasureText("ENTER YOUR NAME", 40) / 2, SCREEN_HEIGHT / 4, 40, WHITE);
    DrawText(TextFormat("Score: %lld", score), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Score: %lld", score), 25) / 2, SCREEN_HEIGHT / 3, 25, YELLOW);

    // Draw name input box
    Rectangle nameBox = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 200, 40};
    DrawRectangleRec(nameBox, DARKGRAY);
    DrawRectangleLinesEx(nameBox, 2, WHITE);

    // Draw current name
    DrawText(menu->playerName, SCREEN_WIDTH / 2 - MeasureText(menu->playerName, 30) / 2, SCREEN_HEIGHT / 2 - 10, 30, WHITE);

    // Draw cursor
    if ((int)(GetTime() * 2) % 2) {
        int cursorX = SCREEN_WIDTH / 2 + MeasureText(menu->playerName, 30) / 2;
        DrawText("_", cursorX, SCREEN_HEIGHT / 2 - 10, 30, WHITE);
    }

    DrawText("Type your name, ENTER to confirm", 10, SCREEN_HEIGHT - 30, 20, GRAY);
}

void UpdatePauseMenu(MenuData *menu) {
    if (IsKeyPressed(KEY_DOWN)) {
        menu->selectedPauseItem = (menu->selectedPauseItem + 1) % PAUSE_ITEM_COUNT;
    }
    if (IsKeyPressed(KEY_UP)) {
        menu->selectedPauseItem = (menu->selectedPauseItem - 1 + PAUSE_ITEM_COUNT) % PAUSE_ITEM_COUNT;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        switch (menu->selectedPauseItem) {
            case PAUSE_ITEM_RESUME:
                menu->currentState = GAME_PLAYING;
                break;
            case PAUSE_ITEM_RESTART:
                menu->currentState = GAME_PLAYING;
                break;
            case PAUSE_ITEM_MAIN_MENU:
                menu->currentState = GAME_MENU;
                break;
            case PAUSE_ITEM_SETTINGS:
                menu->currentState = GAME_SETTINGS;
                break;
            default:
                break;
        }
    }

    // Handle back button click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Vector2 mousePos = GetMousePosition();
      // Convert to virtual coordinates for scaled UI
      float zoom = fminf((float)GetScreenWidth() / SCREEN_WIDTH, (float)GetScreenHeight() / SCREEN_HEIGHT);
      Vector2 offset = {(GetScreenWidth() - SCREEN_WIDTH * zoom) / 2.0f, (GetScreenHeight() - SCREEN_HEIGHT * zoom) / 2.0f};
      Vector2 virtualMouse = {(mousePos.x - offset.x) / zoom, (mousePos.y - offset.y) / zoom};
      Rectangle backButton = {SCREEN_WIDTH - 120, SCREEN_HEIGHT - 50, 100, 30};
      if (CheckCollisionPointRec(virtualMouse, backButton)) {
        menu->currentState = GAME_MENU;
      }
    }
}

void DrawPauseMenu(MenuData *menu) {
    // Light dimmed overlay (instead of heavy black)
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.2f));

    // Large pause indicator
    DrawText("GAME PAUSED", SCREEN_WIDTH / 2 - MeasureText("GAME PAUSED", 60) / 2, SCREEN_HEIGHT / 4, 60, YELLOW);

    // Draw pause menu items
    for (int i = 0; i < PAUSE_ITEM_COUNT; i++) {
        Color color = (i == menu->selectedPauseItem) ? YELLOW : WHITE;
        int y = SCREEN_HEIGHT / 2 + i * MENU_ITEM_SPACING;
        DrawText(pauseItems[i], SCREEN_WIDTH / 2 - MeasureText(pauseItems[i], MENU_ITEM_SIZE) / 2, y, MENU_ITEM_SIZE, color);
    }

    // Back button
    Rectangle backButton = {SCREEN_WIDTH - 120, SCREEN_HEIGHT - 50, 100, 30};
    DrawRectangleRec(backButton, DARKGRAY);
    DrawRectangleLinesEx(backButton, 2, WHITE);
    DrawText("BACK", backButton.x + (backButton.width - MeasureText("BACK", 20)) / 2, backButton.y + (backButton.height - 20) / 2, 20, WHITE);

    // Navigation instructions
    DrawText("Use UP/DOWN arrows to navigate, ENTER to select", 10, SCREEN_HEIGHT - 30, 20, GRAY);
}

void UpdateEnterNameScreen(MenuData *menu) {
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (strlen(menu->playerName) < 19)) {
            menu->playerName[strlen(menu->playerName)] = (char)key;
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && strlen(menu->playerName) > 0) {
        menu->playerName[strlen(menu->playerName) - 1] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER) && strlen(menu->playerName) > 0) {
        // Save score and go to leaderboard
        // This will be handled by leaderboard system
        menu->currentState = GAME_LEADERBOARD;
    }
}