#include "../headers/Knight.h"
#include "../headers/HelperFunctions.h"

std::pair< std::vector<Square *>, std::vector<DirectionalSquares>> Knight::moveable_squares(Square *) {
    FDirector fDirector(m_color);
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square, &fDirector,
                                                       {{Direction::UPUPLEFT,       &FDirector::upupleft},
                                                        {Direction::UPUPRIGHT,      &FDirector::upupright},
                                                        {Direction::DOWNDOWNLEFT,   &FDirector::downdownleft},
                                                        {Direction::DOWNDOWNRIGHT,  &FDirector::downdownright},
                                                        {Direction::UPLEFTLEFT,     &FDirector::upleftleft},
                                                        {Direction::UPRIGHTGIHT,    &FDirector::uprightright},
                                                        {Direction::DOWNLEFTLEFT,   &FDirector::downleftleft},
                                                        {Direction::DOWNRIGHTRIGHT, &FDirector::downrightright}},
                                                       true);
}

bool Knight::fide35(std::vector<DirectionalSquares> &, Square *) { return true; }