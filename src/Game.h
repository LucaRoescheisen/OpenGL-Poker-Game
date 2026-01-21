#ifndef GAME_H
#define GAME_H
#include <stdbool.h>

typedef struct Game {
    int numOfPlayers;
    int highestBet;
    int potSize;
    int activePlayers;
    int roundActionsPerformed;
    int foldCount;
    bool isRunning;
    bool predicting;
    int smallBindPlayer;
    int bigBindPlayer;
    int dealer;
    bool playersAllin;
    int roundsPlayed;
    int order;
} Game;



#endif //GAME_H