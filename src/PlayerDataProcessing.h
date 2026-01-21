
#ifndef PLAYERDATAPROCESSING_H
#define PLAYERDATAPROCESSING_H
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char* playerName;
    int credits;
    bool dealer;
} Player;

Player* InitialisePlayers();
Player* GetPlayerStructArr();
void DestroyPlayers();

#endif //PLAYERDATAPROCESSING_H
