#include "Deck.h"

#include <string.h>
#include <time.h>
#include "Macro.h"

int cardCount = 0;

void GenerateDeck(Card *deck) {
    int i = 0;
    for (int suit = 0; suit < 4; suit++) {
        for (int rank = TWO; rank <= ACE; rank++) {
            deck[i].suit = suit;
            deck[i].rank = rank;
            i++;
        }
    }
    printf("Deck Generated\n");
}

void ShuffleDeck(Card *deck) {
    cardCount = 0; //Deck is shuffled at start of every round so we reset cardCount for when drawing a card

    for (int i = NUM_CARDS - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }

}

Card DrawCard(Card *deck) {
    if (cardCount >= NUM_CARDS) {
        fprintf(stderr, "Error: Tried to draw more cards than available in deck\n");
        exit(EXIT_FAILURE);
    }
    cardCount++;
    return deck[cardCount - 1];
}

char ReturnSuit(Suit c) {
    if (c == 0) { return 'H';}
    if (c == 1) { return 'D';}
    if (c == 2) { return 'C';}
    if (c == 3) { return 'S';}
}