
#ifndef PROCESSUSERINPUTS_H
#define PROCESSUSERINPUTS_H
#include <stdbool.h>
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, int button, int action, int mods);
void cursorCallback(GLFWwindow* window, double posX, double posY);
double* GetMousePosition();
bool GetHasLeftClicked();
#endif //PROCESSUSERINPUTS_H
