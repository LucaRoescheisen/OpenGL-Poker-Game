#include "GameState.h"
#include "textTexture.h"
#include "App.h"
#include "UserInterface.h"

#include "Player.h"
#include "Prediction.h"
#include "HandSolver.h"
#include "UIColourChange.h"

#include "MainMenuFunctions.h"


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define MAX_RECTANGLES 9
#define COMM_CARDS 5

int pastNumOfPlayers = 0;
float sx = 2.0f / WINDOW_WIDTH;
float sy = 2.0f / WINDOW_HEIGHT;

void CheckGameContinue(struct Player* head, PokerApp* app);
int CompareHands(struct Player* best, struct Player* current);
void RemovePlayers(PokerApp* app);


void HandleMainMenu(PokerApp* app){
    Rectangle* rectList = GetRectStructArr();

    AttemptStartGame(app, rectList);
    SetupTextShader(app);

    RenderText("Welcome to GU Keep'Em", -1 + 880 * sx, 1 - 50 * sy, sx, sy, app);
    RenderText("Enter amount of players (max 8): ", -1 + 8 * sx, 1 - 90 * sy, sx, sy, app);

    ChangeSelectedColor(0);
    RenderUI(0, rectList);
    RenderText(rectList[0].buffer, -1 + rectList[0].x * sx, 0.01 - (rectList[0].y > 540 ? 540 - rectList[0].y : 0 - rectList[0].y) * sy, sx, sy, app);

    app->numberOfPlayers = rectList[0].numberOfPlayers;

    if (app->numberOfPlayers >= 2) {
        RenderPlayerInputFields(app, rectList);
        RenderCreditsInput(app, rectList);
        RenderStartGameButton(app, rectList);
    }
}


void HandleGameLoop(PokerApp* app){
    Rectangle* pokerCards = GetPokerStructArr();
    Rectangle* rectList = GetRectStructArr();
    //Check whether to skip a player
    while (app->currentPlayer->status == FOLDED || app->currentPlayer->status == ALL_IN_BOX || app->currentPlayer==app->lastRaiser) {
        app->currentPlayer = app->currentPlayer->nextPlayer;
    }

    float offsetY = 35;
    float offsetX = 350;

    //Render Player credits, status and name
    RenderText("Players:", -1 + 1400 * sx, 1 - 50 * sy, sx, sy, app);
    RenderText("Credits:", -1 +  (1400 + offsetX)  * sx, 1 - 50 * sy, sx, sy, app);
    struct Player* headCounter = app->head;
    for (int i = 0; i < app->numberOfPlayers; i++) {
        char currentCreditsBuffer[30];
        char playerStatusBuffer[100];
        if (headCounter->status == FOLDED) {
            sprintf(currentCreditsBuffer, "%s %s", headCounter->name, "(FOLDED)");
        }
        else if (headCounter->status == ALL_IN) {
            sprintf(currentCreditsBuffer, "%s %s", headCounter->name, "(ALL_IN)");
        }
        else {
            sprintf(currentCreditsBuffer, "%s ", headCounter->name);
        }
        RenderText(currentCreditsBuffer, -1 + 1400 * sx , 1 - (50 + offsetY) * sy, sx, sy, app);
        snprintf(currentCreditsBuffer, sizeof(currentCreditsBuffer), "%.2f", headCounter->credits);
        RenderText(currentCreditsBuffer, -1 + (1400 + offsetX) * sx , 1 - (50 + offsetY) * sy, sx, sy, app);
        offsetY += 35;
        headCounter = headCounter->nextPlayer;
    }

    //Initialise the Setup of the game
    if (!app->initialisedSetup) {
        app->order = (app->numberOfPlayers > 2) ? (app->dealer + 3) % app->numberOfPlayers : (app->dealer + 1) % app->numberOfPlayers;
        app->bigBlindPlayer = (app->smallBlindPlayer+1) % app->numberOfPlayers;
        app->roundsPlayed = 0;
        app->roundActions= 0;
        app->activePlayers = app->numberOfPlayers;

        app->currentPlayer = FindPlayer(app->head, app->order);

        InitialiseUIRects();
        SetupBlinds(&app->head, &app->smallBlindPlayer, &app->bigBlindPlayer, app->numberOfPlayers, app);
        for (int i = 0; i < COMM_CARDS; i++) {
            app->communityCards.commDeck[i] = DrawCard(app->deck);

            pokerCards[i].cardRankOffset = app->communityCards.commDeck[i].rank - 2;

            pokerCards[i].cardSuitOffset = app->communityCards.commDeck[i].suit;
            //Debugging
            printf("%f\n",pokerCards[i].cardRankOffset);
            printf("%f\n",pokerCards[i].cardSuitOffset);
        }
        //Debugging
        printf("Cards: %s %s\n", RankToString(app->communityCards.commDeck[0].rank), SuitToString(app->communityCards.commDeck[0].suit));
        printf("Cards: %s %s\n", RankToString(app->communityCards.commDeck[1].rank), SuitToString(app->communityCards.commDeck[1].suit));
        printf("Cards: %s %s\n", RankToString(app->communityCards.commDeck[2].rank), SuitToString(app->communityCards.commDeck[2].suit));
        printf("Cards: %s %s\n", RankToString(app->communityCards.commDeck[3].rank), SuitToString(app->communityCards.commDeck[3].suit));
        printf("Cards: %s %s\n", RankToString(app->communityCards.commDeck[4].rank), SuitToString(app->communityCards.commDeck[4].suit));
        //Give each player their card
        for (int i = 0; i < app->numberOfPlayers; i++) {
            for (int j = 0; j < PLAYER_CARDS; j++) {
                FindPlayer(app->head, i)->card[j] = DrawCard(app->deck);

            }
        }
        app->initialisedSetup = true;
    }
    //Render current Player
    char displayNameBuffer[200] = "Current Player: ";
    strcat(displayNameBuffer, app->currentPlayer->name);
    RenderText(displayNameBuffer, -850 * sx, 455* sy, sx, sy, app);
    CheckGameContinue(app->head, app);

    //Check how many community cards to render and then render them
    int cardsToRender = 0;
    if (app->roundsPlayed == 1) {
        cardsToRender = 3;
    }
    else if (app->roundsPlayed == 2) {
        cardsToRender = 4;
    }
    else if (app->roundsPlayed == 3) {
        cardsToRender = 5;
    }

    for (int i = 0; i < cardsToRender; i++) {

        RenderUI(i, pokerCards);
    }

    //Render Player cards
    for (int i = COMM_CARDS; i < COMM_CARDS + PLAYER_CARDS; i++) {
        pokerCards[i].cardRankOffset = app->currentPlayer->card[6 - i].rank - 2;
        pokerCards[i].cardSuitOffset = app->currentPlayer->card[6 - i].suit;
        RenderUI(i, pokerCards);
    }

    //Render UI
    for (int i = 0; i < 4; i++) {
        UIPlayColours(i, app);
        RenderUI(i, rectList);
        if (i == 2) {
            RenderText(rectList[i].buffer, -0.13 - 1 +(170+  rectList[0].x) * sx, 1 - 655 * sy, sx, sy, app);
        }
        //Raising
        if (rectList[i].type == NEXT_PLAYER && rectList[i].nextPlayer) {
            int enteredBet = rectList[i - 1].credits;
            if (enteredBet > 0 && enteredBet <= app->currentPlayer->credits - app->highestBet && enteredBet > MIN_BET) {
                app->lastRaiser = app->currentPlayer;
                //app->roundsPlayed = 1;
                app->currentPlayer->credits -= enteredBet;
                app->highestBet += enteredBet;
                app->currentPlayer->betValue = app->highestBet;
                app->currentPlayer = app->currentPlayer->nextPlayer;
                app->potSize += enteredBet;
                rectList[i-1].buffer[0] = '\0';
                rectList[2].validToGoTONextPlayer = false;
            }
            rectList[i].nextPlayer = false;
        }

    }

    //Checks if next player is eligible
    if ( rectList[2].credits < app->currentPlayer->credits && rectList[2].credits > 0) {
        rectList[2].validToGoTONextPlayer = true;
    }


    //Checks if player folds
    if (rectList[1].hasFolded) {
        app->currentPlayer->status = FOLDED;
        app->currentPlayer = app->currentPlayer->nextPlayer;
        app->activePlayers--;
        //app->roundActions++;
        rectList[1].hasFolded= false;
    }

    //Checks if player raises
    if (rectList[0].hasCalled ) {
        if (app->currentPlayer->credits == app->highestBet) {
            app->roundActions++;
            app->currentPlayer = app->currentPlayer->nextPlayer;
            rectList[0].hasCalled = false;
        }
        if (app->currentPlayer->credits > app->highestBet){
            if (app->roundActions == app->activePlayers -1 && app->roundsPlayed == 0) {
                printf("Rounds Played: %i\n", app->roundsPlayed);
                app->roundActions++;
                app->currentPlayer = app->currentPlayer->nextPlayer;
                rectList[0].hasCalled = false;
            }
            else if (app->currentPlayer->credits > app->highestBet && app->currentPlayer->betValue != app->highestBet) {
                printf("Bet Value: %f\n", app->currentPlayer->betValue);
                printf("Highest Bet: %i\n", app->highestBet);
                int difference = app->highestBet - app->currentPlayer->betValue;
                if (difference > 0) {
                    app->currentPlayer->credits -= difference;
                    app->potSize += difference;
                    app->currentPlayer->betValue = app->highestBet;
                }
            }
            app->roundActions++;
            app->currentPlayer = app->currentPlayer->nextPlayer;
            rectList[0].hasCalled = false;

        }
    }

    //Renders an  All-in box if player has less than the highest bet
    if (app->highestBet > app->currentPlayer->credits) {
        RenderUI(4, rectList);
        RenderText("All-in", -1 + 1420 * sx, 1 - 715 * sy, sx, sy, app);
    }

    //Checks if All-in has been pressed
    if (rectList[4].allIn) {
        app->potSize += app->currentPlayer->credits;

        app->currentPlayer->credits = 0;
        app->activePlayers--;
        app->currentPlayer->status = ALL_IN;
        app->currentPlayer = app->currentPlayer->nextPlayer;
        rectList[4].allIn = false;
    }

    //Render UI Text
    RenderText("Call/Check ", -1 + 1530 * sx, 1 - 535 * sy, sx, sy, app);
    RenderText("Fold", -1 + 1560 * sx, 1 - 595 * sy, sx, sy, app);
    RenderText("Raise", -1 + 1300 * sx, 1 - 655 * sy, sx, sy, app);
    RenderText("Confirm", -1 + 1812 * sx, 1 - 655 * sy, sx, sy, app);
    char potSizeBuffer[100];
    sprintf(potSizeBuffer, "Pot size: %i", app->potSize);
    RenderText(potSizeBuffer, -1 + 1400 * sx,   100 * sy, sx, sy, app);
    char highestBetBuffer[100];
    sprintf(highestBetBuffer, "Highest Bet: %i", app->highestBet);
    RenderText(highestBetBuffer, -1 + 1400 * sx,   70 * sy, sx, sy, app);
    char currentBetBuffer[100];
    sprintf(currentBetBuffer, "Current Bet: %f", app->currentPlayer->betValue);
    RenderText(currentBetBuffer, -1 + 1400 * sx,   40 * sy, sx, sy, app);


}


void HandlePlayerWinner(PokerApp* app){

    static struct Player* winner = NULL;

    //Initialise end-game UI
    if (!app->winnerCardsToRender) {
        InitialiseRectsGame();
        InitialiseUIEndGame();
        app->winnerCardsToRender = true;

    }


    Rectangle* pokerCards = GetPokerStructArr();
    Rectangle* rectList = GetRectStructArr();
    rectList[app->numberOfPlayers].type = CONFIRM_NEW_GAME;
    int tiePlayers[6] = {0};
    int tieCount = 0;

    //Finds player with best hand
    if (!app->foundWinner) {
        struct Player* current = app->head;
        struct Player* best = NULL;
        int tiePlayers[8] = {0};
        int tieCount = 0;
        int playerIndex = 0;

        for (int i = 0; i < app->numberOfPlayers; i++) {
            if (current->status == FOLDED) {
                current = current->nextPlayer;
                playerIndex++;
                continue;
            }

            FindBestHand(current, app->communityCards.commDeck);

            if (best == NULL || CompareHands(best, current) < 0) {
                best = current;
                tiePlayers[0] = playerIndex;
                tieCount = 1;
            }
            else if (CompareHands(best, current) == 0) {
                tiePlayers[tieCount++] = playerIndex;
            }

            current = current->nextPlayer;
            playerIndex++;
        }

        if (tieCount > 1) {
            //printf("Tie! The winners are: ");
            for (int i = 0; i < tieCount; i++) {
                struct Player* p = FindPlayer(app->head, tiePlayers[i]);
                p->credits += app->potSize / tieCount;
                printf("%s, ", p->name);
            }

        }
        else {
            winner = FindPlayer(app->head, tiePlayers[0]);
            winner->credits += app->potSize;
        }

        app->foundWinner = true;
        if (winner != NULL) {
            for (int i = 0; i < COMM_CARDS; i++) {
                pokerCards[i].cardRankOffset = winner->bestHand[i].rank - 2;
                pokerCards[i].cardSuitOffset =  winner->bestHand[i].suit;
            }
        }

    }

    //Prints winner, their cards, and the amount they won
    if (winner != NULL) {
        char printWinningPlayer[200] = "Winning Player: ";
        strcat(printWinningPlayer, winner->name);
        RenderText(printWinningPlayer, -850 * sx, 455* sy, sx, sy, app);
        char winningAmount[200];
        sprintf(winningAmount, "You have won: %d", app->potSize);
        RenderText(winningAmount, -850 * sx, 410* sy, sx, sy, app);
    }
    else {
        //Prints the winners if there is a tie
        RenderText("It's a tie!", -850 * sx, 455 * sy, sx, sy, app);
        char tieNames[256] = "Winners: ";
        for (int i = 0; i < tieCount; i++) {
            struct Player* p = FindPlayer(app->head, tiePlayers[i]);
            strcat(tieNames, p->name);
            if (i < tieCount - 1) {
                strcat(tieNames, ", ");
            }
        }
        RenderText(tieNames, -850 * sx, 410 * sy, sx, sy, app);
        char msg[100];
        sprintf(msg, "Pot split: %d", app->potSize / tieCount);
        RenderText(msg, -850 * sx, 365 * sy, sx, sy, app);
    }


    for (int i = 0; i < COMM_CARDS; i++) {
        RenderUI(i, pokerCards);
    }


    //Creates  leave boxes for each player
    int offset = 0;
    RenderText("Select box to leave", 610 * sx, (20)* sy, sx, sy, app);
    for (int i = 0; i <= app->numberOfPlayers ; i++) {
        SelectedColor(i, app);
        RenderUI(i, rectList);
        if (i < app->numberOfPlayers) { RenderText(FindPlayer(app->head, i)->name, 610 * sx, (-42 - offset)* sy, sx, sy, app); }
       // if (i == app->numberOfPlayers) {RenderText("Confirm", 610 * sx, (-42- offset)* sy, sx, sy, app);}
        offset += 61;

    }

    //Switches the state
    if (rectList[app->numberOfPlayers].startGame) {
        app->gameState.state = STATE_RESET;

    }


}

void HandleReset(PokerApp* app){
    Rectangle* rectList = GetRectStructArr();
    //Resets requires variables
    app->foundWinner = false;
    RemovePlayers(app);
    app->initialisedSetup = false;
    app->winnerCardsToRender = false;
    app->roundsPlayed = 0;
    app->roundActions = 0;
    app->potSize = 0;
    app->highestBet = 0;

    //Terminates game if there are less than 2 players
   if (app->numberOfPlayers < 2) {
        glfwTerminate();
       exit(EXIT_SUCCESS);
   }
    for (int i = 0; i < app->numberOfPlayers; i++) {
        FindPlayer(app->head, i)->betValue = 0;
        FindPlayer(app->head, i)->status = ACTIVE;
    }

    app->dealer = (app->dealer + 1) % app->numberOfPlayers;
    app->smallBlindPlayer = (app->dealer + 1) % app->numberOfPlayers;
    app->bigBlindPlayer = (app->smallBlindPlayer + 1) % app->numberOfPlayers;
    printf("Dealer is: %i", app->dealer);
    printf("Small blind pler: : %i", app->smallBlindPlayer);
    printf("Big blind pler: : %i", app->bigBlindPlayer);
    for (int i = 0; i < app->numberOfPlayers+1; i++) {
        if (rectList[i].vao != 0) {
            DestroyRectangle(&rectList[i]);

        }
    }
    printf("Number of Players: %i", app->numberOfPlayers);
    app->gameState.state = STATE_SETUP;
}

StateHandler stateHandlers[] = {
   [STATE_MAIN_MENU] = HandleMainMenu,
   [STATE_SETUP] = HandleGameLoop,
   [STATE_DETERMINE_WINNER] = HandlePlayerWinner,
   [STATE_RESET] = HandleReset,
};





void RemovePlayers(PokerApp* app) {
    Rectangle* rectList = GetRectStructArr();
    if (app->head == NULL) return;

    printf("Players with 0 credits will be automatically removed.\n");

    struct Player* current = app->head;
    struct Player* prev = NULL;
    int count = app->numberOfPlayers;
    int removed = 0;
    for (int i = 0; i < count; ) {
        struct Player* next = current->nextPlayer;

        if (current->credits < BIG_GUESS) {
            printf("%s has 0 credits and will be removed.\n", current->name);
            RemovePlayer(&app->head, current->num);
            app->numberOfPlayers--;
            removed = 1;
        } else {
            if (rectList[i].selectedToLeave) {
                printf("%s chose to leave and will be removed.\n", current->name);
                RemovePlayer(&app->head, current->num);
                app->numberOfPlayers--;
                removed = 1;
            }
        }

        i++;

        if (app->head == NULL) {break;}
        current = next;
    }

    if (app->head != NULL) {
        ReassignPlayerIndex(app->head);
    }

}





    //This function is a cluster fuck held together by masking tape
    //nvm fixed

void CheckGameContinue(struct Player* head, PokerApp* app, CommunityCards* c) {
        Rectangle* rectList = GetRectStructArr();
        Rectangle* pokerCards = GetPokerStructArr();
        if(app->roundActions >= app->activePlayers || app->currentPlayer == app->lastRaiser || app->activePlayers == 1) {
            app->lastRaiser = NULL;
            app->roundsPlayed++;
            app->roundActions = 0;
            app->order = (app->dealer + 1) % app->numberOfPlayers;
            app->activePlayers = 0;
            for (int i = 0; i < app->numberOfPlayers; i++) {
                if (FindPlayer(head, i)->status == ACTIVE) {
                    app->activePlayers++;
                }
            }
            // If only one active player left or we've done all betting rounds, end
            if (app->activePlayers <= 1 || app->roundsPlayed > 3) {
                app->gameState.state = STATE_DETERMINE_WINNER;

            }
        }
}

int CompareHands(struct Player* best, struct Player* current) {
        if (best->handStrength > current->handStrength) { return 1;}
        if (best->handStrength < current->handStrength) { return -1;}
        if (best->highCard  > current->highCard) { return 1;}
        if (best->highCard  < current->highCard) { return -1;}

        //Tie break computation!!!WOOOoowOOOOoOoWWWW
        for (int i = 0; i < 5; i++) {
            if (best->bestHand[i].rank > current->bestHand[i].rank) { return 1;}
            if (best->bestHand[i].rank < current->bestHand[i].rank) { return -1;}
        }
        return 0;
    }