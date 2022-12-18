#include <utility>

#include "../headers/PGNGameDetails.h"

PGNGameDetails::PGNGameDetails(
        std::vector<std::string> moves,
        std::string event,
        std::string site,
        std::string date,
        std::string round,
        std::string white_name,
        std::string black_name,
        std::string result,
        std::string whiteElo,
        std::string blackElo,
        std::string eco,
        std::string time_control,
        std::string termination
) {
    this->moves = std::move(moves);
    this->event = std::move(event);
    this->site = std::move(site);
    this->date = std::move(date);
    this->round = std::move(round);
    this->white_name = std::move(white_name);
    this->black_name = std::move(black_name);
    this->result = std::move(result);
    this->whiteElo = std::move(whiteElo);
    this->blackElo = std::move(blackElo);
    this->eco = std::move(eco);
    this->time_control = std::move(time_control);
    this->termination = std::move(termination);
}

std::string PGNGameDetails::getMove(int index) const {
    return moves.at(index);
}

unsigned long PGNGameDetails::getMoveCount() const {
    return moves.size();
}

std::string PGNGameDetails::getEvent() const { return event; }

std::string PGNGameDetails::getSite() const { return site; }

std::string PGNGameDetails::getDate() const { return date; }

std::string PGNGameDetails::getRound() const { return round; }

std::string PGNGameDetails::getWhiteName() const { return white_name; }

std::string PGNGameDetails::getBlackName() const { return black_name; }

std::string PGNGameDetails::getResult() const { return result; }

std::string PGNGameDetails::getWhiteElo() const { return whiteElo; }

std::string PGNGameDetails::getBlackElo() const { return blackElo; }

std::string PGNGameDetails::getECO() const { return eco; }

std::string PGNGameDetails::getTimeControl() const { return time_control; }

std::string PGNGameDetails::getTermination() const { return termination; }