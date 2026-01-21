
#ifndef CHECKMALLOC_H
#define CHECKMALLOC_H
#include <stdio.h>
#include <stdlib.h>
#define MAX_PLAYER_NAME_LEN 200
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define MAX_RECTANGLES 11
#define MAX_PLAYER_NUM 6
#define NUM_CARDS 52
#define PLAYER_CARDS 2
#define COMM_CARDS 5
#define MIN_BET 10
#define BIG_GUESS 100
#define CHECK_MALLOC(ptr)                        \
do {                                         \
if ((ptr) == NULL) {                     \
fprintf(stderr, "Malloc failed\n");  \
exit(EXIT_FAILURE);                  \
}                                        \
} while (0)
#define GLCall(x) GLClearError();\
x;\
GLCheckError(#x, __FILE__, __LINE__)

#endif //CHECKMALLOC_H
