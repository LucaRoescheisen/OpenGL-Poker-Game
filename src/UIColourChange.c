#include "App.h"
#include "UserInterface.h"
#include "UIColourChange.h"
bool AllNamesAreValid(PokerApp* app) {
        if (app->head != NULL) return true;
        Rectangle* rectList = GetRectStructArr();
        app->credits = atoi(rectList[9].buffer);
        for (int i = 0; i < app->numberOfPlayers + 1; i++) {
            if (rectList[i].type == PLAYER_NAME_READ_WRITE) {
                AddPlayer(&app->head, i - 1, app->credits, rectList[i].buffer);

                if (rectList[i].buffer[0] == '\0') {
                    rectList[8].startGame = false;
                    return false;
                }
            }
        }

        app->currentPlayer = app->head;
        return true;
    }


void ChangeSelectedColor(int i) {
    Rectangle* rectList = GetRectStructArr();
    glUseProgram(rectList[i].shader->m_RendererID);
    int selectedCol0 = glGetUniformLocation(rectList[i].shader->m_RendererID, "u_Color");

    if (rectList[i].focused) {
        glUniform4f(selectedCol0, 0.8f, 0.8f, 0.8f, 1.0f);
    }
    else {
        glUniform4f(selectedCol0, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}



void UIPlayColours(int i, PokerApp* app) {
    Rectangle* rectList = GetRectStructArr();
    glUseProgram(rectList[i].shader->m_RendererID);
    int selectedCol0 = glGetUniformLocation(rectList[i].shader->m_RendererID, "u_Color");

    if (rectList[i].focused) {
        if (rectList[i].type == RAISE) {
            if (rectList[i].credits > app->head->credits ) {
                glUniform4f(selectedCol0, 1.0f, 0.5f, 1.3f, 1.0f);
            }
            else {
                glUniform4f(selectedCol0, 1.0f, 0.843f, 2.0f, 1.0f);
            }
        }
        else {
            glUniform4f(selectedCol0, 1.0f, 0.843f, 2.0f, 1.0f);
        }
    }
    else {
        glUniform4f(selectedCol0, 1.0f, 0.843f, 3.0f, 1.0f);
    }
}

void SelectedColor(int i, PokerApp* app) {
        Rectangle* rectList = GetRectStructArr();
        glUseProgram(rectList[i].shader->m_RendererID);
        int selectedCol0 = glGetUniformLocation(rectList[i].shader->m_RendererID, "u_Color");
        if (rectList[i].selectedToLeave == true) {

            glUniform4f(selectedCol0, 1.0f, 0.5f, 1.3f, 1.0f);
        }
        else {
            glUniform4f(selectedCol0, 1.0f, 0.843f, 3.0f, 1.0f);

        }
}

