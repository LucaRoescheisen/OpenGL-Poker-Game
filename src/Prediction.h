#ifndef PREDICTION_H
#define PREDICTION_H
#include "Game.h"
#include "Player.h"
#include "App.h"

//void DisplayCommunityCards();
//void DisplayPlayers(struct Player* head, Game *g);
//void HandleAction(int actionType, struct Player* head, Game *g);
void SetupBlinds(struct Player** head, int* smallBlindPlayer, int* bigBlindPlayer, int numberOfPlayers, PokerApp* app);
const char* RankToString(int rank);
const char* SuitToString(int suit);

#endif //PREDICTION_H
