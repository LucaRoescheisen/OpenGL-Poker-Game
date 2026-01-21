#include "PlayerDataProcessing.h"
#include "glfw/glfw3.h"
#include "string.h"
#include "ProcessUserInputs.h"
#include "UserInterface.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Macro.h"

#define MAX_PLAYER_NAME_LENGTH  20
#include "Macro.h"
//Globals
double mousePositions[2];
int numberOfPlayers;
bool hasClicked = false;


int x = 0;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Player* playerList = glfwGetWindowUserPointer(window);
    Rectangle* rectList = GetRectStructArr();


    bool anyFocused = false;
    int selectedIndex = 0;
    for (int i = 0; i < MAX_RECTANGLES; i++) {
        if (rectList[i].focused) {
            anyFocused = true;
            selectedIndex = i;
            break;
        }
    }
    if (!anyFocused) return;

    if (action== GLFW_PRESS && rectList[selectedIndex].type == PLAYER_NAME_READ_WRITE  && rectList[selectedIndex].rendered) {

        if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z ) {
            char c = key;
            if (x == 0) {
                snprintf(rectList[selectedIndex].buffer, MAX_PLAYER_NAME_LENGTH, "%c", c);
            }
            else {
                snprintf(rectList[selectedIndex].buffer + strlen(rectList[selectedIndex].buffer), MAX_PLAYER_NAME_LENGTH - strlen(rectList[selectedIndex].buffer), "%c",  c);
            }
        }
        printf("%s\n", rectList[selectedIndex].buffer);
    }
    x++;

    if (key == GLFW_KEY_BACKSPACE) {
        if (rectList[selectedIndex].type == PLAYER_NAME_READ_WRITE || rectList[selectedIndex].type == RAISE) {
            rectList[selectedIndex].buffer[0] = '\0';

        }
    }

    if (action== GLFW_PRESS && rectList[selectedIndex].type == PLAYER_NUM_READ_WRITE) {
        if (key >= GLFW_KEY_2 && key <= GLFW_KEY_8) {

            char c = key;
            int digit = c - '0';
            rectList[selectedIndex].numberOfPlayers = digit;

            snprintf(rectList[selectedIndex].buffer, MAX_PLAYER_NAME_LENGTH, "%i", digit);
        }
    }
    if (action== GLFW_PRESS && (rectList[selectedIndex].type == CREDITS_READ_WRITE ||  rectList[selectedIndex].type == RAISE )) {
        if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {

            int digit = key - GLFW_KEY_0;
            size_t len = strlen(rectList[selectedIndex].buffer);
            rectList[selectedIndex].buffer[len] = '0' + digit;
            rectList[selectedIndex].buffer[len + 1] = '\0';
            rectList[selectedIndex].credits = atoi(rectList[selectedIndex].buffer);
        }
    }




}

void mouseCallback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS ) {
        Rectangle* rectList = GetRectStructArr();
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        yPos = (WINDOW_HEIGHT / 2.0) - yPos;
        for (int i = 0; i < MAX_RECTANGLES; i++) {
            if ( xPos >= rectList[i].x && xPos <= rectList[i].x + rectList[i].w && yPos >= rectList[i].y && yPos <= rectList[i].y + rectList[i].h) {
                rectList[i].focused = true;

                if (i == 10) {

                    rectList[i].startGame = true;
                }
                if (rectList[i].type == NEXT_PLAYER && rectList[2].validToGoTONextPlayer) {
                    rectList[i].nextPlayer = true;
                    printf("HMMMMMM");
                }
                if (rectList[i].type == FOLD) { rectList[i].hasFolded = true; }
                if (rectList[i].type == CALL_CHECK) { rectList[i].hasCalled = true; }
                if (rectList[i].type == ALL_IN_BOX) {
                    printf("FUKC YEAHHHHH");
                    rectList[i].allIn = true;
                }

                if (rectList[i].type == PLAYER_STAY_OR_LEAVE) {
                    rectList[i].selectedToLeave = true;

                }
                if (rectList[i].type ==CONFIRM_NEW_GAME) {rectList[i].startGame = true; }
            }
            else {

                rectList[i].focused = false;
            }
        }


    }
}

void cursorCallback(GLFWwindow* window, double posX, double posY) {


}
bool GetHasLeftClicked() {
    return hasClicked;
}

int GetNumberOfPlayers() {
    return numberOfPlayers;
}

double* GetMousePosition() {
    return mousePositions;
}