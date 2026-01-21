
#include "UserInterface.h"
#include "glad/glad.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Macro.h"
#include "Player.h"


int originalPos[4] = {368,445,400,28};

void RenderRect(Rectangle* rect);

void UpdatePokerCardVertexData(Rectangle* rect);
Rectangle CreatePokerCard(int x, int y, int w, int h);

Rectangle* textboxes = NULL;
Rectangle* pokerCards = NULL;


unsigned int rectIndicies[] = {
    0,1,2,
    2,3,0
};

void RenderUI(int index, Rectangle* rect) {
    if (rect[index].vao != 0) {

            RenderRect(&rect[index]);
    }
}



void DestroyUI() {

    for (int i = 0; i < MAX_RECTANGLES; i++) {
        DestroyRectangle(&textboxes[i]);

    }
    free(textboxes);

}

void DestroyPokerCards() {
    for (int i = 0; i < MAX_RECTANGLES; i++) {
        DestroyRectangle(&pokerCards[i]);

    }
    free(pokerCards);
}


void InitialiseRectsMainMenu() {
    int offset = 0;
    textboxes = malloc(sizeof(Rectangle) * MAX_RECTANGLES);
    CHECK_MALLOC(textboxes);
    memset(textboxes, 0, sizeof(Rectangle) * MAX_RECTANGLES);
    Shader* sharedShader = ConstructShader("D:/University/Poker/res/shaders/Basic.shader");
    Texture* sharedTexture = ConstructTexture("D:/University/Poker/res/images/pokercards.jpg");


    for (int i = 0; i < MAX_RECTANGLES; i++) {
        textboxes[i] = CreateRectangle(368,445 - offset,400,28);
        textboxes[i].buffer = malloc( 20 * sizeof(char) );
        textboxes[i].shader = sharedShader;
        textboxes[i].texture = sharedTexture;
        textboxes[i].buffer[0] = '\0';
        textboxes[i].focused = false;
        textboxes[i].rendered = false;
        if (i > 0 && i <= 8) { textboxes[i].type = PLAYER_NAME_READ_WRITE; }
        else if (i ==9) { textboxes[i].type = CREDITS_READ_WRITE; textboxes[i].credits = 0; }
        else if (i ==10) { textboxes[i].type = START_GAME; textboxes[i].startGame = false;  }
        offset += 60;

    }

    textboxes[0].type = PLAYER_NUM_READ_WRITE;

}



void InitialiseUIRects() {
    if (textboxes != NULL) {
        free(textboxes);
    }
    int offset = 0;
    textboxes = malloc(sizeof(Rectangle) * MAX_RECTANGLES);
    CHECK_MALLOC(textboxes);
    memset(textboxes, 0, sizeof(Rectangle) * MAX_RECTANGLES);
    Shader* sharedShader = ConstructShader("D:/University/Poker/res/shaders/Basic.shader");
    Texture* sharedTexture = ConstructTexture("D:/University/Poker/res/images/pokercards.jpg");


    for (int i = 0; i < 5; i++) {
        if (i == 3){  textboxes[i] = CreateRectangle(1810, 0- offset + 60,90,28);}
       else if ( i == 4) {  textboxes[i] = CreateRectangle(1400, 0- offset + 60,90,28);}
        else { textboxes[i] = CreateRectangle(1400, 0- offset,400,28); }

        textboxes[i].buffer = malloc( 20 * sizeof(char) );
        textboxes[i].shader = sharedShader;
        textboxes[i].texture = sharedTexture;
        textboxes[i].buffer[0] = '\0';
        textboxes[i].focused = false;
        textboxes[i].rendered = true;
        textboxes[i].nextPlayer = false;
        textboxes[i].validToGoTONextPlayer = false;
        textboxes[i].hasFolded = false;
        textboxes[i].hasCalled = false;
        textboxes[i].allIn = false;
        if (i == 0) {textboxes[i].type = CALL_CHECK;}
        if (i == 1) {textboxes[i].type = FOLD;}
        if (i == 2) {textboxes[i].type = RAISE;}
        if (i == 3) {textboxes[i].type = NEXT_PLAYER;}
        if (i == 4) {textboxes[i].type = ALL_IN_BOX;}

        offset += 60;
    }
}


void InitialiseUIEndGame() {
    if (textboxes != NULL) {
        free(textboxes);
    }
    int offset = 0;
    textboxes = malloc(sizeof(Rectangle) * MAX_RECTANGLES);
    CHECK_MALLOC(textboxes);
    memset(textboxes, 0, sizeof(Rectangle) * MAX_RECTANGLES);
    Shader* sharedShader = ConstructShader("D:/University/Poker/res/shaders/Basic.shader");
    Texture* sharedTexture = ConstructTexture("D:/University/Poker/res/images/pokercards.jpg");


    for (int i = 0; i < MAX_RECTANGLES; i++) {
        textboxes[i] = CreateRectangle(1600, -50 - offset,50,28);
        textboxes[i].shader = sharedShader;
        textboxes[i].texture = sharedTexture;
        textboxes[i].focused = false;
        textboxes[i].rendered = true;
        textboxes[i].playerHasLeft = false;
        textboxes[i].selectedToLeave = false;
        textboxes[i].startGame = false;
        textboxes[i].type = PLAYER_STAY_OR_LEAVE;
        offset += 60;
    }
}


void InitialiseRectsGame() {
    if (pokerCards != NULL) {
        free(pokerCards);
    }
    int offset = 0;
    int handOffset = 0;
    pokerCards = malloc(sizeof(Rectangle) * (COMM_CARDS + PLAYER_CARDS));
    CHECK_MALLOC(pokerCards);
    memset(pokerCards, 0, sizeof(Rectangle) * (COMM_CARDS + PLAYER_CARDS));
    Shader* sharedShader = ConstructShader("D:/University/Poker/res/shaders/Card.shader");
    Texture* sharedTexture = ConstructTexture("D:/University/Poker/res/images/pokercards.jpg");


    for (int i = 0; i <  COMM_CARDS + PLAYER_CARDS; i++) {

        if (i >= COMM_CARDS) {
            pokerCards[i] = CreatePokerCard(100 + handOffset ,-500 ,240,380);
            handOffset += 250;
        }
        else {
            pokerCards[i] = CreatePokerCard(100 + offset ,0 ,240,380);
        }

        pokerCards[i].buffer = malloc( 20 * sizeof(char) );
        pokerCards[i].shader = sharedShader;
        pokerCards[i].texture = sharedTexture;
        pokerCards[i].buffer[0] = '\0';
        pokerCards[i].focused = false;
        pokerCards[i].rendered = false;
        pokerCards[i].isPokerCard = true;

        offset += 250;

    }


}


Rectangle* GetPokerStructArr() {
    return pokerCards;
}


Rectangle* GetRectStructArr() {
    return textboxes;
}

void UpdateRectVertexData(Rectangle* rect) {
    const float x0 = (rect->x  /(float)WINDOW_WIDTH) *2.0f -1.0f;
    const float y0 = (rect->y  /(float)WINDOW_HEIGHT) *2.0f;
    const float w0 = (rect->w  /(float)WINDOW_WIDTH) *2.0f;
    const float h0 = (rect->h  /(float)WINDOW_HEIGHT) *2.0f;

    float positions[] = {
        x0,         y0,
        x0 + w0,    y0,
        x0 + w0,    y0 + h0,
        x0,         y0 + h0,

    };
    for (int i = 0; i < 8; i++) {
        rect->positions[i] = positions[i];
    }

}

void UpdatePokerCardVertexData(Rectangle* rect) {
    const float x0 = (rect->x  /(float)WINDOW_WIDTH) *2.0f -1.0f;
    const float y0 = (rect->y  /(float)WINDOW_HEIGHT) *2.0f;
    const float w0 = (rect->w  /(float)WINDOW_WIDTH) *2.0f;
    const float h0 = (rect->h  /(float)WINDOW_HEIGHT) *2.0f;

    float positions[] = {
        x0,         y0,       0,  0,
        x0 + w0,    y0,       1.0f,  0,
        x0 + w0,    y0 + h0,  1.0f,  1.0f,
        x0,         y0 + h0,  0,  1.0f

    };
    for (int i = 0; i < 16; i++) {
        rect->positions[i] = positions[i];
    }

}








Rectangle CreateRectangle(int x, int y, int w, int h) {
    Rectangle rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    UpdateRectVertexData(&rect);


    glGenVertexArrays(1, &rect.vao);
    glBindVertexArray(rect.vao);

    rect.vbo = CreateVertexBuffer(rect.positions, sizeof(float)*2*4);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    rect.ib = CreateIndexBuffer(rectIndicies, 6);



    return rect;

};

Rectangle CreatePokerCard(int x, int y, int w, int h) {
    Rectangle rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    UpdatePokerCardVertexData(&rect);


    glGenVertexArrays(1, &rect.vao);
    glBindVertexArray(rect.vao);

    rect.vbo = CreateVertexBuffer(rect.positions, sizeof(float)*4*4);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));
    rect.ib = CreateIndexBuffer(rectIndicies, 6);



    return rect;

};



void RenderRect(Rectangle* rect) {


    glBindVertexArray(rect->vao);
    BindShader(rect->shader);


    glUseProgram(rect->shader->m_RendererID);

    glUniform1i(glGetUniformLocation(rect->shader->m_RendererID, "u_Texture"), 0);
    BindTexture(rect->texture, 0);
    BindIndexBuffer(rect->ib);

    if (rect->isPokerCard) {

        glUniform1i(glGetUniformLocation(rect->shader->m_RendererID, "u_column"), rect->cardRankOffset);
        glUniform1i(glGetUniformLocation(rect->shader->m_RendererID, "u_row"), rect->cardSuitOffset);
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void DestroyRectangle(Rectangle* rect) {

    glDeleteVertexArrays(1, &rect->vao);
    UnbindShader(rect->shader);
    UnbindTexture(rect->texture);
    UnbindIndexBuffer(rect->ib);
    UnbindVertexBuffer(rect->vbo);

    DeleteTexture(rect->texture);

}


