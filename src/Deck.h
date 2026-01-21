#ifndef DECK_H
#define DECK_H

typedef enum {
    TWO = 2,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE

 }Rank;

typedef enum {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
}Suit;


typedef struct {
    Suit suit;
    Rank rank;
}Card;

typedef struct {
    Card commDeck[5];
}CommunityCards;


void GenerateDeck(Card *deck);
void ShuffleDeck(Card *deck);
Card DrawCard(Card *deck);
char ReturnSuit(Suit c);
#endif //DECK_H
