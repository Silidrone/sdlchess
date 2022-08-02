#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "MTexture.h"
#include "Board.h"
#include "SharedData.h"

class Game {
private:
    Game();
    ~Game();

public:
    static Game &instance() {
        static Game INSTANCE;
        return INSTANCE;
    }

    void init();
    void run();
    void over();
protected:
    SharedData& sharedData;
    Board *m_board;
};

#endif //CHESS_GAME_H