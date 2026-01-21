#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "GameState.h"

#include "textTexture.h"
#include "Shader.h"
#include "Texture.h"
#include "Player.h"
#include "Deck.h"
#include "PlayerDataProcessing.h"

struct PokerApp{
    GLFWwindow* window;
    Renderer* renderer;
    GameState gameState;
    Shader* textShader;
    TextTexture textTexture;
    Texture* cardAtlas;

    struct Player* head;
    struct Player* currentPlayer;
    struct Player* lastRaiser;
    Card deck[NUM_CARDS];
    CommunityCards communityCards;

    bool winnerCardsToRender;
    bool initialisedSetup;
    bool foundWinner;
    int smallBlindPlayer;
    int bigBlindPlayer;
    int dealer;
    int foldCount;
    int potSize;
    int highestBet;
    int order;
    int numberOfPlayers;
    float credits;

    int roundsPlayed;
    int activePlayers;
    int roundActions;

    unsigned int textVBO;
    unsigned int textVAO;
    unsigned int in_v4_coordLocation; // in vec4 coord at location
    float sx, sy;

};





bool PokerAppInit(PokerApp* app);
void PokerAppDestroy(PokerApp* app);
void GameLoop(PokerApp* app);

PokerApp* GetPokerApp();


#endif // APP_H
