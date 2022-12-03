#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "MTexture.h"
#include "Board.h"
#include "SharedData.h"
#include "MoveLogger.h"

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
    bool m_game_over;
    ChessColor m_turn_color;
};

#endif //CHESS_GAME_H