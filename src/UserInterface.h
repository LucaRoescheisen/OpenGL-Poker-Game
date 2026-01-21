
#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "stdbool.h"

enum Type {
    PLAYER_NUM_READ_WRITE,
    PLAYER_NAME_READ_WRITE,
    CREDITS_READ_WRITE,
    START_GAME,
    RAISE,
    CALL_CHECK,
    FOLD,
    NEXT_PLAYER,
    ALL_IN_BOX,
    PLAYER_STAY_OR_LEAVE,
    CONFIRM_NEW_GAME
};


typedef struct {
    char* buffer;
    unsigned int vao;
    VertexBuffer* vbo;
    IndexBuffer* ib;
    Shader* shader;
    Texture* texture;
    bool focused;
    bool rendered;
    enum Type type;
    enum SubType subType;
    int x,y,w,h;
    float positions[16];
    int numberOfPlayers;
    int credits;
    bool startGame;
    bool nextPlayer;

    bool playerHasLeft;

    bool selectedToLeave;

    bool allIn;
    bool hasCalled;
    bool hasFolded;
    bool validToGoTONextPlayer;
    float cardRankOffset;
    float cardSuitOffset;
    bool isPokerCard;
    float uOffset;
    float vOffset;

}Rectangle;


Rectangle CreateRectangle(int x, int y, int w, int h);
Rectangle* GetRectStructArr();
Rectangle* GetPokerStructArr();
void RenderRectangle(Rectangle* rect);
void DestroyRectangle(Rectangle* rect);
void InitialiseRectsGame();
void InitialiseUIRects();
void InitialiseUIEndGame();
void InitialiseRectsMainMenu();
void RenderUI(int index, Rectangle* rect);
void DestroyUI();
void DestroyPokerCards();
#endif //USERINTERFACE_H
