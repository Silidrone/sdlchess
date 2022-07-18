#ifndef CHESS_CHESSCOLORED_H
#define CHESS_CHESSCOLORED_H

enum class ChessColor {
    BLACK,
    WHITE
};

class ChessColored {
public:
    explicit ChessColored(ChessColor color) : m_color(color) {}

    virtual ~ChessColored() = 0;

    ChessColored(const ChessColored &other) = default;

    ChessColored& operator=(const ChessColored &other) = default;

    ChessColor getColor() const { return m_color; }

protected:
    ChessColor m_color;
};

ChessColored::~ChessColored() = default;

#endif //CHESS_CHESSCOLORED_H
