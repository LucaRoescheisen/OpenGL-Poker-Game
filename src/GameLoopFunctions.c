#include "GameLoopFunctions.h"
#include "Prediction.h"
#include "UIColourChange.h"
void SkipInactivePlayers(PokerApp* app) {
    while (app->currentPlayer->status == FOLDED || app->currentPlayer->status == ALL_IN_BOX) {
        app->currentPlayer = app->currentPlayer->nextPlayer;
    }
}
void RenderPlayerInfo(PokerApp* app) {
    float sx_n = 2.0f / WINDOW_WIDTH;
    float sy_n = 2.0f / WINDOW_HEIGHT;
    float offsetY = 35;
    float offsetX = 350;

    RenderText("Players:", -1 + 1400 * sx_n, 1 - 50 * sy_n, sx_n, sy_n, app);
    RenderText("Credits:", -1 + (1400 + offsetX) * sx_n, 1 - 50 * sy_n, sx_n, sy_n, app);

    struct Player* head = app->head;

    for (int i = 0; i < app->numberOfPlayers; i++) {
        char label[30];
        if (head->status == FOLDED) {
            sprintf(label, "%s (FOLDED)", head->name);
        } else if (head->status == ALL_IN) {
            sprintf(label, "%s (ALL_IN)", head->name);
        } else {
            sprintf(label, "%s", head->name);
        }

        RenderText(label, -1 + 1400 * sx_n, 1 - (50 + offsetY) * sy_n, sx_n, sy_n, app);

        char creditBuffer[30];
        snprintf(creditBuffer, sizeof(creditBuffer), "%.2f", head->credits);
        RenderText(creditBuffer, -1 + (1400 + offsetX) * sx_n, 1 - (50 + offsetY) * sy_n, sx_n, sy_n, app);

        offsetY += 35;
        head = head->nextPlayer;
    }
}

void InitialiseGame(PokerApp* app, Rectangle* pokerCards) {
    app->order = (app->numberOfPlayers > 2) ? (app->dealer + 3) % app->numberOfPlayers : (app->dealer + 1) % app->numberOfPlayers;
    app->bigBlindPlayer = (app->smallBlindPlayer + 1) % app->numberOfPlayers;
    app->roundsPlayed = 0;
    app->roundActions = 0;
    app->activePlayers = app->numberOfPlayers;

    app->currentPlayer = FindPlayer(app->head, app->order);

    InitialiseUIRects();
    SetupBlinds(&app->head, &app->smallBlindPlayer, &app->bigBlindPlayer, app->numberOfPlayers, app);

    for (int i = 0; i < COMM_CARDS; i++) {
        app->communityCards.commDeck[i] = DrawCard(app->deck);
        pokerCards[i].cardRankOffset = app->communityCards.commDeck[i].rank - 2;
        pokerCards[i].cardSuitOffset = app->communityCards.commDeck[i].suit;
    }

    for (int i = 0; i < app->numberOfPlayers; i++) {
        for (int j = 0; j < PLAYER_CARDS; j++) {
            FindPlayer(app->head, i)->card[j] = DrawCard(app->deck);
        }
    }

    app->initialisedSetup = true;
}

void RenderCurrentPlayer(PokerApp* app) {
    float sx_n = 2.0f / WINDOW_WIDTH;
    float sy_n = 2.0f / WINDOW_HEIGHT;
    char buffer[200] = "Current Player: ";
    strcat(buffer, app->currentPlayer->name);
    RenderText(buffer, -850 * sx_n, 455 * sy_n, sx_n, sy_n, app);
}

void RenderCommunityCards(PokerApp* app, Rectangle* pokerCards) {
    int cardsToRender = 0;
    if (app->roundsPlayed == 1) cardsToRender = 3;
    else if (app->roundsPlayed == 2) cardsToRender = 4;
    else if (app->roundsPlayed == 3) cardsToRender = 5;

    for (int i = 0; i < cardsToRender; i++) {
        RenderUI(i, pokerCards);
    }
}

void RenderPlayerCards(PokerApp* app, Rectangle* pokerCards) {
    for (int i = COMM_CARDS; i < COMM_CARDS + PLAYER_CARDS; i++) {
        pokerCards[i].cardRankOffset = app->currentPlayer->card[6 - i].rank - 2;
        pokerCards[i].cardSuitOffset = app->currentPlayer->card[6 - i].suit;
        RenderUI(i, pokerCards);
    }
}

void HandleUIInteraction(PokerApp* app, Rectangle* rectList) {
    float sx_n = 2.0f / WINDOW_WIDTH;
    float sy_n = 2.0f / WINDOW_HEIGHT;
    for (int i = 0; i < 4; i++) {
        UIPlayColours(i, app);
        RenderUI(i, rectList);
        if (i == 2) {
            RenderText(rectList[i].buffer, -0.13 - 1 +(170+  rectList[0].x) * sx_n, 1 - 655 * sy_n, sx_n, sy_n, app);
        }

        if (rectList[i].type == NEXT_PLAYER && rectList[i].nextPlayer) {
            int enteredBet = rectList[i - 1].credits;
            if (enteredBet > 0 && enteredBet <= app->currentPlayer->credits && enteredBet > MIN_BET) {
                app->lastRaiser = app->currentPlayer;
                app->currentPlayer->credits -= enteredBet;
                app->highestBet += enteredBet;
                app->currentPlayer->betValue = app->highestBet;
                app->currentPlayer = app->currentPlayer->nextPlayer;
                app->potSize += enteredBet;
                rectList[i - 1].buffer[0] = '\0';
                rectList[2].validToGoTONextPlayer = false;
            }
            rectList[i].nextPlayer = false;
        }
        _sleep(2);
    }

    if (rectList[2].credits < app->currentPlayer->credits && rectList[2].credits > 0) {
        rectList[2].validToGoTONextPlayer = true;
    }

    if (rectList[1].hasFolded) {
        app->currentPlayer->status = FOLDED;
        app->currentPlayer = app->currentPlayer->nextPlayer;
        app->activePlayers--;
        rectList[1].hasFolded = false;
    }

    if (rectList[0].hasCalled && app->currentPlayer->credits > app->highestBet) {
        HandleCall(app);
        rectList[0].hasCalled = false;
    }

    if (app->highestBet > app->currentPlayer->credits) {
        RenderUI(4, rectList);
        RenderText("All-in", -1 + 1420 * sx_n, 1 - 715 * sy_n, sx_n, sy_n, app);
    }

    if (rectList[4].allIn) {
        app->potSize += app->currentPlayer->credits;
        app->currentPlayer->credits = 0;
        app->activePlayers--;
        app->currentPlayer->status = ALL_IN;
        app->currentPlayer = app->currentPlayer->nextPlayer;
        rectList[4].allIn = false;
    }
}

void HandleCall(PokerApp* app) {
    if (app->roundActions == app->activePlayers - 1 && app->roundsPlayed == 0) {
        app->roundActions++;
        app->currentPlayer = app->currentPlayer->nextPlayer;
    } else {
        int diff = app->highestBet - app->currentPlayer->betValue;
        if (diff > 0) {
            app->currentPlayer->credits -= diff;
            app->potSize += diff;
            app->currentPlayer->betValue = app->highestBet;
        }
        app->roundActions++;
        app->currentPlayer = app->currentPlayer->nextPlayer;
    }
}

void RenderActionLabels(PokerApp* app) {
    float sx_n = 2.0f / WINDOW_WIDTH;
    float sy_n = 2.0f / WINDOW_HEIGHT;
    RenderText("Call/Check ", -1 + 1530 * sx_n, 1 - 535 * sy_n, sx_n, sy_n, app);
    RenderText("Fold", -1 + 1560 * sx_n, 1 - 595 * sy_n, sx_n, sy_n, app);
    RenderText("Raise", -1 + 1300 * sx_n, 1 - 655 * sy_n, sx_n, sy_n, app);
    RenderText("Confirm", -1 + 1812 * sx_n, 1 - 655 * sy_n, sx_n, sy_n, app);
}

void RenderPotInfo(PokerApp* app) {
    float sx_n = 2.0f / WINDOW_WIDTH;
    float sy_n = 2.0f / WINDOW_HEIGHT;
    char buffer[100];
    sprintf(buffer, "Pot size: %i", app->potSize);
    RenderText(buffer, -1 + 1400 * sx_n, 100 * sy_n, sx_n, sy_n, app);

    sprintf(buffer, "Highest Bet: %i", app->highestBet);
    RenderText(buffer, -1 + 1400 * sx_n, 70 * sy_n, sx_n, sy_n, app);

    sprintf(buffer, "Current Bet: %f", app->currentPlayer->betValue);
    RenderText(buffer, -1 + 1400 * sx_n, 40 * sy_n, sx_n, sy_n, app);
}
