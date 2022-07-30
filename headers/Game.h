#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "MTexture.h"
#include "Board.h"
#include "SharedData.h"

class Game {
public:
    Game();
    ~Game();
    void init();
    void run();
protected:
    SharedData& sharedData;
    Board *m_board;
};

#endif //CHESS_GAME_H