#include <string>
#include <vector>

#ifndef CHESS_PGNGAMEDETAILS_H
#define CHESS_PGNGAMEDETAILS_H

class PGNGameDetails {
public:
    explicit PGNGameDetails(
            std::vector<std::string> moves,
            std::string event,
            std::string site,
            std::string date,
            std::string round,
            std::string white_name,
            std::string black_name,
            std::string result,
            std::string whiteElo = "",
            std::string blackElo = "",
            std::string eco = "",
            std::string time_control = "",
            std::string termination = ""
    );

    std::string getMove(int) const;
    unsigned long getMoveCount() const;

    std::string getEvent() const;

    std::string getSite() const;

    std::string getDate() const;

    std::string getRound() const;

    std::string getWhiteName() const;

    std::string getBlackName() const;

    std::string getResult() const;

    std::string getWhiteElo() const;

    std::string getBlackElo() const;

    std::string getECO() const;

    std::string getTimeControl() const;

    std::string getTermination() const;

protected:
    std::vector<std::string> moves;
    std::string event, site, date, round, white_name, black_name, result, whiteElo, blackElo, eco, time_control, termination;
};

#endif //CHESS_PGNGAMEDETAILS_H
