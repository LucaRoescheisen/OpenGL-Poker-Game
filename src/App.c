#include <glad/glad.h>
#include "App.h"
#include <stdio.h>
#include <stdbool.h>

#include <time.h>
#include "textTexture.h"
#include "Shader.h"
#include "UserInterface.h"
#include "PlayerDataProcessing.h"
#include "ProcessUserInputs.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define MAX_PLAYER_NAME_LENGTH 20







void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

void GLCheckError(const char* function, const char* file, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("[OpenGL ERROR] (0x%x): %s %s:%d\n", error, function, file, line);
    }
}


bool PokerAppInit(PokerApp* app) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    app->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Poker Game ", NULL, NULL);
    if (!app->window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return false;
    }
    glfwSetKeyCallback(app->window, keyCallback);
    glfwSetMouseButtonCallback(app->window, mouseCallback);
    glfwSetCursorPosCallback(app->window, cursorCallback);
    glfwMakeContextCurrent(app->window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return false;
    }


    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    return true;
}

void PokerAppDestroy(PokerApp* app) {
    if (app->window) {
        glfwDestroyWindow(app->window);
    }
    glfwTerminate();
}





void GameLoop(PokerApp* app){
    srand(time(NULL));
    const float sx = 2.0f / WINDOW_WIDTH;
    const float sy = 2.0f / WINDOW_HEIGHT;

    //--------------------------------------Create Text atlas and Bind text---------------//
    FT_Library ft;
    FT_Face face;
    FT_Init_FreeType(&ft);
    if (FT_New_Face(ft, "D:/University/Poker/res/fonts/Arial.ttf", 0, &face)) {
        printf("Could not load font\n");
    }
    app->textTexture = GenerateTextureAtlas(face);
    app->textShader = ConstructShader("D:/University/Poker/res/shaders/text.shader");
    GLCall(glGenVertexArrays(1, & app->textVAO));
    GLCall(glBindVertexArray( app->textVAO));
    GLCall(glGenBuffers(1, & app->textVBO));
    app->in_v4_coordLocation =  glGetAttribLocation(app->textShader->m_RendererID, "coord");


    glDisable(GL_DEPTH_TEST);
    //-----------------------------------------------------------------------------------------//
        app->smallBlindPlayer = 1;
        app->potSize = 0;
        app->dealer = 0;
        app->foldCount = 0;
        app->roundsPlayed = 0;
        app->initialisedSetup = false;
        app->foundWinner = false;
        app->winnerCardsToRender = false;
    //-----------------Initialise player linked list pointer---------------//
        app->head = NULL;
        app->currentPlayer = NULL;
        app-> lastRaiser = NULL;
    //-----------------Initialise deck and community cards----------------//
    GenerateDeck(app->deck);
    ShuffleDeck(app->deck);


    glfwSetWindowUserPointer(app->window, GetPlayerStructArr());
    //Initialise UI rectangles
    InitialiseRectsMainMenu();
    InitialiseRectsGame();

    while(!glfwWindowShouldClose(app->window)){
        GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GLCall(glUseProgram(app->textShader->m_RendererID));
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        BindShader(app->textShader);
        stateHandlers[app->gameState.state](app);



        // Swap buffers and poll events
        glfwSwapBuffers(app->window);
        glfwPollEvents();
    }
    DeleteTexture(app->cardAtlas);
    DestroyPlayers(app->numberOfPlayers);
    DeleteShader(app->textShader);
    DestroyUI();
    DestroyPokerCards();

    glfwDestroyWindow(app->window);
    glfwTerminate();

}
