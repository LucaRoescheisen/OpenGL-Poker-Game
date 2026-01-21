#include "MainMenuFunctions.h"
#include "UIColourChange.h"




void AttemptStartGame(PokerApp* app, Rectangle* rectList) {
    float sx_n = 2.0f / WINDOW_WIDTH;
    float sy_n = 2.0f / WINDOW_HEIGHT;
    if (rectList[10].startGame && AllNamesAreValid(app)) {
        if (rectList[9].credits > 0) {
            app->credits = rectList[9].credits;
            app->gameState.state = STATE_SETUP;
            app->activePlayers = 0;
            for (int i = 0; i < MAX_RECTANGLES; i++) {
                if (rectList[i].vao != 0) {
                    DestroyRectangle(&rectList[i]);
                }
            }
        }
    }
}

 void SetupTextShader(PokerApp* app) {
    glUniform1i(glGetUniformLocation(app->textShader->m_RendererID, "u_Texture"), 0);
    glUniform4f(glGetUniformLocation(app->textShader->m_RendererID, "u_Color"), 0.0f, 0.0f, 0.0f, 1.0f);
}

 void RenderPlayerInputFields(PokerApp* app, Rectangle* rectList) {
    float sx_n = 2.0f / WINDOW_WIDTH;
    float sy_n = 2.0f / WINDOW_HEIGHT;
    float offset = 0.1f;
    for (int i = 1; i < app->numberOfPlayers + 1; i++) {
        rectList[i].rendered = true;
        char label[20];
        snprintf(label, sizeof(label), "Player %d", i);

        RenderText(label, -0.13 - 1 + rectList[0].x * sx_n, -offset - (rectList[0].y > 540 ? 540 - rectList[0].y : 0 - rectList[0].y) * sy_n, sx_n, sy_n, app);
        ChangeSelectedColor(i);
        RenderUI(i, rectList);
        RenderText(rectList[i].buffer, -1 + rectList[i].x * sx_n, 0.01 - (rectList[i].y > 540 ? 540 - rectList[i].y : 0 - rectList[i].y) * sy_n, sx_n, sy_n, app);
        offset += 0.11f;
    }
}

 void RenderCreditsInput(PokerApp* app, Rectangle* rectList) {
    float sx_n = 2.0f / WINDOW_WIDTH;
    float sy_n = 2.0f / WINDOW_HEIGHT;
    const int i = 9;
    ChangeSelectedColor(i);
    RenderUI(i, rectList);
    RenderText("Credits", -0.13 - 1 + rectList[i].x * sx_n, 0.008 - (rectList[i].y > 540 ? 540 - rectList[i].y : 0 - rectList[i].y) * sy_n, sx_n, sy_n, app);
    RenderText(rectList[i].buffer, -1 + rectList[i].x * sx_n, 0.01 - (rectList[i].y > 540 ? 540 - rectList[i].y : 0 - rectList[i].y) * sy_n, sx_n, sy_n, app);
}

 void RenderStartGameButton(PokerApp* app, Rectangle* rectList) {
    float sx_n = 2.0f / WINDOW_WIDTH;
    float sy_n = 2.0f / WINDOW_HEIGHT;
    const int i = 10;
    ChangeSelectedColor(i);
    RenderUI(i, rectList);
    RenderText("StartGame", -0.13 - 1 + rectList[i].x * sx_n, 0.008 - (rectList[i].y > 540 ? 540 - rectList[i].y : 0 - rectList[i].y) * sy_n, sx_n, sy_n, app);
}