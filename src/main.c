#include "App.h"

int main() {

    PokerApp app = {0};
    if (!PokerAppInit(&app)) {
        return -1;
    }
    app.gameState.state = STATE_MAIN_MENU;

    GameLoop(&app);

    PokerAppDestroy(&app);

}
