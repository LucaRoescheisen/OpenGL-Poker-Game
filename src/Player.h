#ifndef PLAYER_H
#define PLAYER_H
#include "Macro.h"
#include "Deck.h"
#define KICKER_SIZE 5
enum playerStatus {
    FOLDED = 0,
    ACTIVE ,
    ALL_IN ,
    QUIT,


};

struct Player {
    char name[MAX_PLAYER_NAME_LEN];
    int num;
    Card card[2];
    float credits;
    float betValue;
    enum playerStatus status;
    struct Player* nextPlayer;
    int handStrength;
    int highCard;
    Card bestHand[5];
};

void AddPlayer(struct Player** head, int playerNumber, float credits, char* name);
void RemovePlayer(struct Player** head, int playerNumber);
struct Player*  FindPlayer(struct Player* head, int playerNumber);
void FreePlayers(struct Player* head);
void ReassignPlayerIndex(struct Player* head);
#endif //PLAYER_H
