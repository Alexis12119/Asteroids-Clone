#include "settings.h"
#include "constants.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define SETTINGS_FILE "data/settings.dat"

void InitSettings(Settings *settings) {
    settings->soundEnabled = true;
    settings->difficulty = DIFFICULTY_NORMAL;
    settings->volume = 50;
    LoadSettings(settings);
}

void LoadSettings(Settings *settings) {
    FILE *file = fopen(SETTINGS_FILE, "rb");
    if (file) {
        fread(settings, sizeof(Settings), 1, file);
        fclose(file);
    }
}

void SaveSettings(Settings *settings) {
    // Create data directory if it doesn't exist
    system("mkdir -p data");

    FILE *file = fopen(SETTINGS_FILE, "wb");
    if (file) {
        fwrite(settings, sizeof(Settings), 1, file);
        fclose(file);
    }
}

void DrawSettings(Settings *settings) {
    // Title
    DrawText("SETTINGS", SCREEN_WIDTH / 2 - MeasureText("SETTINGS", 50) / 2, 50, 50, WHITE);

    int startY = 150;
    int lineHeight = 40;

    // Sound toggle
    DrawText("Sound:", 100, startY, 30, WHITE);
    DrawText(settings->soundEnabled ? "ON" : "OFF", 300, startY, 30, settings->soundEnabled ? GREEN : RED);
    DrawText("Press S to toggle", 450, startY, 20, GRAY);

    // Difficulty
    DrawText("Difficulty:", 100, startY + lineHeight, 30, WHITE);
    const char *diffText[] = {"EASY", "NORMAL", "HARD"};
    Color diffColors[] = {GREEN, YELLOW, RED};
    DrawText(diffText[settings->difficulty], 300, startY + lineHeight, 30, diffColors[settings->difficulty]);
    DrawText("Press D to cycle", 450, startY + lineHeight, 20, GRAY);

    // Volume
    DrawText("Volume:", 100, startY + lineHeight * 2, 30, WHITE);
    DrawText(TextFormat("%d%%", settings->volume), 300, startY + lineHeight * 2, 30, WHITE);
    DrawRectangle(450, startY + lineHeight * 2 + 5, 200, 20, DARKGRAY);
    DrawRectangle(450, startY + lineHeight * 2 + 5, (200 * settings->volume) / 100, 20, GREEN);
    DrawText("Use LEFT/RIGHT arrows", 450, startY + lineHeight * 2 + 30, 20, GRAY);

    // Back button
    Rectangle backButton = {SCREEN_WIDTH - 120, SCREEN_HEIGHT - 50, 100, 30};
    DrawRectangleRec(backButton, DARKGRAY);
    DrawRectangleLinesEx(backButton, 2, WHITE);
    DrawText("BACK", backButton.x + (backButton.width - MeasureText("BACK", 20)) / 2, backButton.y + (backButton.height - 20) / 2, 20, WHITE);

    // Instructions
    DrawText("Click BACK button to return to menu", 10, SCREEN_HEIGHT - 30, 20, GRAY);
}

void UpdateSettings(Settings *settings) {
    if (IsKeyPressed(KEY_S)) {
        settings->soundEnabled = !settings->soundEnabled;
        SaveSettings(settings);
    }

    if (IsKeyPressed(KEY_D)) {
        settings->difficulty = (settings->difficulty + 1) % 3;
        SaveSettings(settings);
    }

    if (IsKeyDown(KEY_LEFT) && settings->volume > 0) {
        settings->volume -= 5;
        if (settings->volume < 0) settings->volume = 0;
        SaveSettings(settings);
    }

    if (IsKeyDown(KEY_RIGHT) && settings->volume < 100) {
        settings->volume += 5;
        if (settings->volume > 100) settings->volume = 100;
        SaveSettings(settings);
    }

    // Back navigation handled by mouse clicks in main.c
}