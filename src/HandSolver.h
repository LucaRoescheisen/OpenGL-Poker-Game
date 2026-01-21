#ifndef HANDSOLVER_H
#define HANDSOLVER_H
#include "HandSolver.h"
#include "Deck.h"
#include "Player.h"


enum HandStrength{
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH

};

typedef struct {
    int strength;
    int highCard;
    Card best5[5];
} HandEval;



void FindBestHand(struct Player* player, CommunityCards* commCards);
#endif //HANDSOLVER_H
