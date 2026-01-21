#include "Player.h"

#include <string.h>


struct Player* CreatePlayer(int playerNumber, float credits, char* name) {
    struct Player* newPlayer = malloc(sizeof(struct Player));
    CHECK_MALLOC(newPlayer);
    strncpy(newPlayer->name, name, sizeof(newPlayer->name) - 1);
    newPlayer->name[sizeof(newPlayer->name) - 1] = '\0';
    newPlayer->num = playerNumber;
    newPlayer->credits = credits;
    newPlayer->status = ACTIVE;
    newPlayer->betValue = 0;
    newPlayer->handStrength = 0;
    newPlayer->highCard = 0;
    newPlayer->nextPlayer = NULL;
    return newPlayer;
}

void AddPlayer(struct Player** head, int playerNumber, float credits, char* name) {

    struct Player* newPlayer = CreatePlayer(playerNumber, credits, name);
    if (*head == NULL) {
        *head = newPlayer;
        newPlayer->nextPlayer = *head;
    }
    else {
        struct Player* currentPlayer = *head;
        while (currentPlayer->nextPlayer != *head) {            //Comment for self
            currentPlayer = currentPlayer->nextPlayer;                  //while loop checks if we have reached the end as that is when it points to the head of the list
        }
        currentPlayer->nextPlayer = newPlayer;
        newPlayer->nextPlayer = *head;
    }

}


void RemovePlayer(struct Player** head, int playerNumber) {
    if (*head == NULL) {return;}
    struct Player* currentPlayer = *head;
    struct Player* prev = NULL;
    if (currentPlayer->num == playerNumber) {
        if (currentPlayer->nextPlayer == currentPlayer) {
            free(currentPlayer);
            *head = NULL;
            return;
        }

        struct Player* last = currentPlayer;
        while (last->nextPlayer != currentPlayer) {
            last = last->nextPlayer;
        }

        last->nextPlayer = currentPlayer->nextPlayer;
        *head = currentPlayer->nextPlayer;
        free(currentPlayer);
        return;
    }
    prev = currentPlayer;
    currentPlayer = currentPlayer->nextPlayer;
    while (currentPlayer != *head && currentPlayer->num != playerNumber) {
        if (currentPlayer->num == playerNumber) break;
        prev = currentPlayer;
        currentPlayer = currentPlayer->nextPlayer;
    }
    if (currentPlayer == *head && currentPlayer->num != playerNumber) {
        return;
    }

    prev->nextPlayer = currentPlayer->nextPlayer;
    free(currentPlayer);
}


struct Player*  FindPlayer(struct Player* head, int playerNumber) {
    if (head == NULL) {
        return NULL; // If the list is empty, return NULL
    }

    struct Player* currentPlayer = head;

    // Traverse the circular list
    do {
        if (currentPlayer->num == playerNumber) {
            return currentPlayer; // Found the player
        }
        currentPlayer = currentPlayer->nextPlayer;
    } while (currentPlayer != head);  // Loop until we return to the head

    return NULL;

}

void ReassignPlayerIndex(struct Player* head) {
    if (head == NULL) { return; }
    struct Player* currentPlayer = head;
    int index = 0;
    do {
        currentPlayer->num = index++;
        currentPlayer = currentPlayer->nextPlayer;
    }while (currentPlayer != head);

}






void FreePlayers(struct Player* head) {
    if (head == NULL) return;

    struct Player* current = head->nextPlayer;
    while (current != head) {
        struct Player* next = current->nextPlayer;
        free(current);
        current = next;
    }
    free(head);
}