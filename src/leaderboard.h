#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <time.h>
#include <stdbool.h>

#define MAX_SCORES 10
#define MAX_NAME_LENGTH 20

typedef struct {
    char name[MAX_NAME_LENGTH];
    long long score;
    time_t date;
} HighScore;

typedef struct {
    HighScore scores[MAX_SCORES];
    int count;
} Leaderboard;

// Leaderboard functions
void InitLeaderboard(Leaderboard *leaderboard);
void LoadLeaderboard(Leaderboard *leaderboard);
void SaveLeaderboard(Leaderboard *leaderboard);
bool AddScore(Leaderboard *leaderboard, const char *name, long long score);
void DrawLeaderboard(Leaderboard *leaderboard);
void UpdateLeaderboard();

#endif