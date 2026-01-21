#include "Prediction.h"
#include <stdbool.h>
#include <stdio.h>




void SetupBlinds(struct Player** head, int* smallBlindPlayer, int* bigBlindPlayer, int numOfPlayers, PokerApp* app) {
    struct Player* t_smallBlindPlayer = FindPlayer(*head, *smallBlindPlayer);
    struct Player* t_bigBlindPlayer = FindPlayer(*head, *bigBlindPlayer);  //NOTE t_ indicates temp variable
    printf("%s bets the small blind of 50\n",  t_smallBlindPlayer->name);
    t_smallBlindPlayer->credits -= 50;
    t_smallBlindPlayer->betValue += 50;
    app->potSize += 50;
    if (*smallBlindPlayer == numOfPlayers - 1) { *smallBlindPlayer = -1; }


    printf("%s bets the big blind of 100\n", t_bigBlindPlayer->name);
    t_bigBlindPlayer->credits -= 100;
    t_bigBlindPlayer->betValue += 100;
    app->potSize += 100;
    app->highestBet = 100;

}







const char* RankToString(int rank) {
    static const char* ranks[] = {
        "?", "?", // 0 and 1 not used
        "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "J", "Q", "K", "A"
    };
    if (rank < 2 || rank > 14) return "?";
    return ranks[rank];
}

const char* SuitToString(int suit) {
    static const char* suits[] = { "C", "D", "H", "S" }; // Clubs, Diamonds, Hearts, Spades
    return suits[suit];
}


void DisplayPlayers(struct Player* head, Game *g) {
    printf("Player             Balance        Bet          Cards\n");
    printf("====================================================\n");

    for (int i = 0; i < g->numOfPlayers; i++) {
        struct Player* current = FindPlayer(head, i);
        if (current == NULL) {
            printf("Error: Player %d not found!\n", i);
            continue;
        }

        char activeMark = (i == g->order) ? '*' : ' ';
        char dealerMark = (i == g->dealer) ? 'D' : ' ';

         char* cardDisplay;
        if (current->status == FOLDED) {
            cardDisplay = "FOLDED";
        } else if (i == g->order) {
            static char buffer[16];
            snprintf(buffer, sizeof(buffer), "%s%s %s%s",
                     RankToString(current->card[0].rank),
                     SuitToString(current->card[0].suit),
                     RankToString(current->card[1].rank),
                     SuitToString(current->card[1].suit));
                     cardDisplay = buffer;

        } else {
            cardDisplay = "*****";
        }


        printf("%c%c %-15s %10.1f %12.1f      %s\n", activeMark, dealerMark, current->name, current->credits, current->betValue, cardDisplay);
    }
}

void FoldAction(struct Player* currentPlayer, Game *g) {
    currentPlayer->status = FOLDED;
    g->activePlayers--;

}

void CallAction(struct Player* currentPlayer, Game *g) {

    float toCall = g->highestBet - currentPlayer->betValue;
    if (currentPlayer->credits >= toCall) {
        currentPlayer->credits -= toCall;
        currentPlayer->betValue += toCall;
        g->potSize += toCall;
        g->roundActionsPerformed++;
    }
    else {
        printf("Not enough credits, but you will go all in: %.0f\n", currentPlayer->credits);
        g->potSize += currentPlayer->credits ;
        currentPlayer->betValue += currentPlayer->credits;
        currentPlayer->credits = 0;
        currentPlayer->status = ALL_IN;
        g->roundActionsPerformed++;
    }
}

void RaiseAction(struct Player* currentPlayer, Game *g) {
    int input = 0;
    int minRaise = 50;
    int maxRaise = currentPlayer->credits + currentPlayer->betValue;
    bool isRaising = true;

    while (isRaising) {
        printf("What do you want to raise to? (min %d, max %d): ", minRaise, maxRaise);
        scanf("%i", &input);
        getchar();
        if (input >= minRaise && input <= maxRaise) {
            isRaising = false;
            break;
        }
        printf("Invalid raise amount.\n");
    }

    float toAdd = input + g->highestBet - currentPlayer->betValue;
    currentPlayer->credits -= toAdd;
    currentPlayer->betValue = input + g->highestBet;
    g->highestBet += input ;
    g->potSize += toAdd;
    g->roundActionsPerformed = 1;


}


/*

void HandleAction(int actionType, struct Player* head, Game *g) {
    switch (actionType) {
        case FOLD:
            FoldAction(head, g);
            break;
        case CALL:
            CallAction(head, g);
            break;
        case RAISE:
            RaiseAction(head, g);
            break;
        case SHOW:
            printf("SHoWING");
            break;
        default:
            printf("Invalid action");
            break;
    }


}
*/