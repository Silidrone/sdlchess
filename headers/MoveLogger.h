#ifndef CHESS_MOVELOGGER_H
#define CHESS_MOVELOGGER_H

#include <vector>
#include "Piece.h"

struct Log {
    int move_count;
    Piece *piece;
    std::string disp;
    std::string prev;
    std::string current;
};

class MoveLogger {
private:
    MoveLogger();

    std::vector<Log> m_logs{};
    bool m_renderActive;
public:
    static MoveLogger &instance();

    void addLog(Piece *, const std::string &, const std::string &, const std::string &);

    int getCurrentMoveCount() const;

    std::vector<Log> getLogs(Piece *);

    void render() const;

    void toggle();
};

#endif
