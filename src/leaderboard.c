#include "leaderboard.h"
#include "constants.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SCORES_FILE "data/scores.dat"

void InitLeaderboard(Leaderboard *leaderboard) {
    leaderboard->count = 0;
    for (int i = 0; i < MAX_SCORES; i++) {
        strcpy(leaderboard->scores[i].name, "EMPTY");
        leaderboard->scores[i].score = 0;
        leaderboard->scores[i].date = 0;
    }
    LoadLeaderboard(leaderboard);
}

void LoadLeaderboard(Leaderboard *leaderboard) {
    FILE *file = fopen(SCORES_FILE, "rb");
    if (file) {
        fread(leaderboard, sizeof(Leaderboard), 1, file);
        fclose(file);
    }
}

void SaveLeaderboard(Leaderboard *leaderboard) {
    // Create data directory if it doesn't exist
    system("mkdir -p data");

    FILE *file = fopen(SCORES_FILE, "wb");
    if (file) {
        fwrite(leaderboard, sizeof(Leaderboard), 1, file);
        fclose(file);
    }
}

bool AddScore(Leaderboard *leaderboard, const char *name, long long score) {
    // Check if score qualifies for leaderboard
    if (leaderboard->count < MAX_SCORES || score > leaderboard->scores[MAX_SCORES - 1].score) {
        // Find insertion point
        int insertPos = -1;
        for (int i = 0; i < MAX_SCORES; i++) {
            if (score > leaderboard->scores[i].score) {
                insertPos = i;
                break;
            }
        }

        if (insertPos == -1 && leaderboard->count < MAX_SCORES) {
            insertPos = leaderboard->count;
        }

        if (insertPos != -1) {
            // Shift lower scores down
            for (int i = MAX_SCORES - 1; i > insertPos; i--) {
                leaderboard->scores[i] = leaderboard->scores[i - 1];
            }

            // Insert new score
            strncpy(leaderboard->scores[insertPos].name, name, MAX_NAME_LENGTH - 1);
            leaderboard->scores[insertPos].name[MAX_NAME_LENGTH - 1] = '\0';
            leaderboard->scores[insertPos].score = score;
            leaderboard->scores[insertPos].date = time(NULL);

            if (leaderboard->count < MAX_SCORES) {
                leaderboard->count++;
            }

            SaveLeaderboard(leaderboard);
            return true;
        }
    }
    return false;
}

void DrawLeaderboard(Leaderboard *leaderboard) {
    // Title
    DrawText("LEADERBOARD", SCREEN_WIDTH / 2 - MeasureText("LEADERBOARD", 50) / 2, 50, 50, WHITE);

    // Column headers
    int startY = 150;
    DrawText("RANK", 50, startY, 25, YELLOW);
    DrawText("NAME", 150, startY, 25, YELLOW);
    DrawText("SCORE", 350, startY, 25, YELLOW);
    DrawText("DATE", 500, startY, 25, YELLOW);

    // Draw scores
    for (int i = 0; i < leaderboard->count; i++) {
        int y = startY + 40 + i * 35;

        // Rank
        DrawText(TextFormat("%d", i + 1), 50, y, 25, WHITE);

        // Name
        DrawText(leaderboard->scores[i].name, 150, y, 25, WHITE);

        // Score
        DrawText(TextFormat("%lld", leaderboard->scores[i].score), 350, y, 25, WHITE);

        // Date
        char dateStr[20];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localtime(&leaderboard->scores[i].date));
        DrawText(dateStr, 500, y, 25, GRAY);
    }

    // Back button
    Rectangle backButton = {SCREEN_WIDTH - 120, SCREEN_HEIGHT - 50, 100, 30};
    DrawRectangleRec(backButton, DARKGRAY);
    DrawRectangleLinesEx(backButton, 2, WHITE);
    DrawText("BACK", backButton.x + (backButton.width - MeasureText("BACK", 20)) / 2, backButton.y + (backButton.height - 20) / 2, 20, WHITE);

    // Instructions
    DrawText("Click BACK button to return to menu", 10, SCREEN_HEIGHT - 30, 20, GRAY);
}

void UpdateLeaderboard() {
    // Back navigation handled by mouse clicks in main.c
}