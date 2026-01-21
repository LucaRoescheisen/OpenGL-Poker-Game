#include "HandSolver.h"
#include "Deck.h"
#include "Player.h"
#include "Prediction.h"
#include <stdbool.h>
#include <string.h>

#define TOTAL_COMBOS 21

void FindHighCard(int rankHist[15], int *highCard);
void Generate5CardCombos(Card* cardInput, Card combos[TOTAL_COMBOS][5]);


void FillTempCardHolder(Card* cards, struct Player* player, CommunityCards* commCards) {
    cards[0] = player->card[0];
    cards[1] = player->card[1];
    for (int i = 0; i < 5; i++) {
        cards[i + 2] = commCards->commDeck[i];
    }
}


HandEval Evaluate5CardHand(Card* hand) {
    HandEval result;
    memset(&result, 0, sizeof(result));


    Card c[5];
    memcpy(c, hand, sizeof(Card) * 5);

    for (int i = 0; i < 4; ++i)
        for (int j = i + 1; j < 5; ++j)
            if (c[j].rank > c[i].rank || (c[j].rank == c[i].rank && c[j].suit > c[i].suit)) {
                Card temp = c[i];
                c[i] = c[j];
                c[j] = temp;
            }


    int rankHist[15] = {0};
    int suitHist[4]  = {0};

    for (int i = 0; i < 5; ++i) {
        rankHist[c[i].rank]++;
        suitHist[c[i].suit]++;
    }

    //Flush check
    int  flushSuit = -1;
    bool isFlush   = false;
    for (int s = 0; s < 4; ++s)
        if (suitHist[s] == 5) { isFlush = true; flushSuit = s; break; }

    //Straight check
    bool isStraight = false;
    int  straightHigh = 0;
    for (int r = ACE; r >= 5; --r) {
        if (rankHist[r]   && rankHist[r - 1] &&
            rankHist[r-2] && rankHist[r - 3] &&
            rankHist[r-4]) {
            isStraight  = true;
            straightHigh = r;
            break;
        }
    }
    //Wheel
    if (!isStraight &&
        rankHist[ACE] && rankHist[5] && rankHist[4] &&
        rankHist[3]  && rankHist[2]) {
        isStraight   = true;
        straightHigh = 5;
    }


    int fourKind = -1;
    int threeKind = -1;
    int pair1 = -1;
    int pair2 = -1;

    for (int r = ACE; r >= 2; --r) {
        if (rankHist[r] == 4)  {fourKind  = r;}
        else if (rankHist[r] == 3) {threeKind = r;}
        else if (rankHist[r] == 2) {
            if (pair1 == -1)  { pair1 = r;}
            else if (pair2 == -1) {pair2 = r;}
        }
    }

    if (isStraight && isFlush) {
        result.strength = (straightHigh == ACE) ? ROYAL_FLUSH : STRAIGHT_FLUSH;
        result.highCard = straightHigh;
    }
    else if (fourKind != -1) {
        result.strength = FOUR_KIND;
        result.highCard = fourKind;
    }
    else if (threeKind != -1 && (pair1 != -1 || pair2 != -1)) {
        result.strength = FULL_HOUSE;
        result.highCard = threeKind;
    }
    else if (isFlush) {
        result.strength = FLUSH;
        result.highCard = c[0].rank;
    }
    else if (isStraight) {
        result.strength = STRAIGHT;
        result.highCard = straightHigh;
    }
    else if (threeKind != -1) {
        result.strength = THREE_KIND;
        result.highCard = threeKind;
    }
    else if (pair1 != -1 && pair2 != -1) {
        result.strength = TWO_PAIR;
        result.highCard = pair1;
    }
    else if (pair1 != -1) {
        result.strength = ONE_PAIR;
        result.highCard = pair1;
    }
    else {
        result.strength = HIGH_CARD;
        result.highCard = c[0].rank;
    }


    memcpy(result.best5, c, sizeof(Card) * 5);
    return result;
}



void FindBestHand(struct Player* player, CommunityCards* commCards) {
    Card cards[7];
    Card combos[21][5];
    FillTempCardHolder(cards, player, commCards);
    Generate5CardCombos(cards, combos);
    printf("Player: \n");
    for (int i = 0; i < 7; i++) {
        printf("Cards: %s %s\n", RankToString(cards[i].rank), SuitToString(cards[i].suit));

    }
    HandEval best = { .strength = -1 };
    for (int i = 0; i < TOTAL_COMBOS; ++i) {
        HandEval e = Evaluate5CardHand(combos[i]);
        if (e.strength > best.strength || (e.strength == best.strength && e.highCard > best.highCard) || (e.strength == best.strength && e.highCard == best.highCard &&memcmp(e.best5, best.best5, sizeof(best.best5)) > 0)) {
            best = e;
            }
    }

    player->handStrength = best.strength;
    player->highCard = best.highCard;
    memcpy(player->bestHand, best.best5, sizeof(best.best5));
}


void Generate5CardCombos(Card* cardsInput, Card combos[TOTAL_COMBOS][5]) {
    int count = 0;
    for (int i = 0; i < 7; i++) {
        for (int j = i + 1; j < 7; j++) {
            int k = 0;
            for (int m = 0; m < 7; m++) {
                if (m != i && m != j) {
                    combos[count][k++] = cardsInput[m];
                }
            }
            count++;
        }
    }

}