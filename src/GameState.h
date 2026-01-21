#ifndef GAMESTATE_H
#define GAMESTATE_H


typedef struct PokerApp PokerApp;

typedef enum {
    STATE_MAIN_MENU,
    STATE_SETUP,
    STATE_DEALING,
    STATE_PLAYER_TURN,
    STATE_PLAYER_BETTING,
    STATE_DETERMINE_WINNER,
    STATE_RESET
} GameStateEnum;

typedef struct {
    GameStateEnum state;
} GameState;

typedef void (*StateHandler)(PokerApp* app);

void HandleMainMenu(PokerApp* app);
void HandleGameLoop(PokerApp* app);
void HandlePlayerWinner(PokerApp* app);
void HandleReset(PokerApp* app);

extern StateHandler stateHandlers[];

#endif // GAMESTATE_H