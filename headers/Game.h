#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "MTexture.h"
#include "Board.h"
#include "SharedData.h"
#include "MoveLogger.h"

class Game {
public:
    Game();
    ~Game();

    void init();
    void run();
private:
    void over();

    SharedData& sharedData;
    MoveLogger m_moveLogger;
    Board m_board;
    ChessColor m_turn_color;
    bool m_game_over;
};

#endif //CHESS_GAME_H