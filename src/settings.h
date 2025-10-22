#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

typedef enum {
    DIFFICULTY_EASY,
    DIFFICULTY_NORMAL,
    DIFFICULTY_HARD
} Difficulty;

typedef struct {
    bool soundEnabled;
    Difficulty difficulty;
    int volume; // 0-100
} Settings;

// Settings functions
void InitSettings(Settings *settings);
void LoadSettings(Settings *settings);
void SaveSettings(Settings *settings);
void DrawSettings(Settings *settings);
void UpdateSettings(Settings *settings);

#endif