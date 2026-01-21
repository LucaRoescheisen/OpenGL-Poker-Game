
#ifndef GAMELOOPFUNCTIONS_H
#define GAMELOOPFUNCTIONS_H
#include "App.h"
#include "UserInterface.h"
void SkipInactivePlayers(PokerApp* app);
void RenderPlayerInfo(PokerApp* app);
void InitialiseGame(PokerApp* app, Rectangle* pokerCards);
void RenderCurrentPlayer(PokerApp* app);
void RenderCommunityCards(PokerApp* app, Rectangle* pokerCards);
void RenderPlayerCards(PokerApp* app, Rectangle* pokerCards);
void HandleUIInteraction(PokerApp* app, Rectangle* rectList);
void HandleCall(PokerApp* app);
void RenderActionLabels(PokerApp* app);
void RenderPotInfo(PokerApp* app);
#endif //GAMELOOPFUNCTIONS_H
