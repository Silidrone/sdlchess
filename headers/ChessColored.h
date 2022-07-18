#ifndef CHESS_CHESSCOLORED_H
#define CHESS_CHESSCOLORED_H

enum class ChessColor {
    BLACK = 0,
    WHITE = 1
};

class ChessColored {
public:
    explicit ChessColored(ChessColor color) : m_color(color) {}

    virtual ~ChessColored() = default;;

    ChessColored(const ChessColored &other) = default;

    ChessColored& operator=(const ChessColored &other) = default;

    ChessColor getColor() const { return m_color; }

protected:
    ChessColor m_color;
};

#endif //CHESS_CHESSCOLORED_H
