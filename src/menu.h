#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include "settings.h"
#include <stdbool.h>

typedef enum {
    GAME_MENU,
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_OVER,
    GAME_LEADERBOARD,
    GAME_SETTINGS,
    GAME_ENTER_NAME
} GameState;

typedef enum {
    MENU_ITEM_PLAY,
    MENU_ITEM_LEADERBOARD,
    MENU_ITEM_SETTINGS,
    MENU_ITEM_EXIT,
    MENU_ITEM_COUNT
} MenuItem;

typedef enum {
    PAUSE_ITEM_RESUME,
    PAUSE_ITEM_RESTART,
    PAUSE_ITEM_MAIN_MENU,
    PAUSE_ITEM_SETTINGS,
    PAUSE_ITEM_COUNT
} PauseItem;

typedef struct {
    GameState currentState;
    MenuItem selectedMenuItem;
    PauseItem selectedPauseItem;
    bool soundEnabled;
    Difficulty difficulty;
    char playerName[20];
    int nameCharIndex;
    bool settingsFromPause;
} MenuData;

// Menu functions
void InitMenu(MenuData *menu);
void UpdateMenu(MenuData *menu);
void DrawMenu(MenuData *menu);
void UpdatePauseMenu(MenuData *menu);
void DrawPauseMenu(MenuData *menu);
void DrawGameOverScreen(long long score, MenuData *menu);
void UpdateGameOverScreen(MenuData *menu);
void DrawEnterNameScreen(long long score, MenuData *menu);
void UpdateEnterNameScreen(MenuData *menu);

#endif