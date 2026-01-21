#include "PlayerDataProcessing.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "Macro.h"
Player* playerList = NULL;



Player*  InitialisePlayers(int numberOfPlayers) {
    playerList = malloc(sizeof(Player) * numberOfPlayers);
    CHECK_MALLOC(playerList);
    memset(playerList, 0, sizeof(Player) * numberOfPlayers);
    for (int i = 0; i < numberOfPlayers; i++) {
        playerList[i].playerName = malloc( 20 * sizeof(char) );
        playerList[i].playerName[0] = '\0';
    }
    return playerList;
}

Player* GetPlayerStructArr() {
    return playerList;
}

void DestroyPlayers(int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        free(playerList[i].playerName);
    }
    free(playerList);
}