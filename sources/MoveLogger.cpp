#include "../headers/MoveLogger.h"

MoveLogger::MoveLogger() : m_logs({}), m_renderActive(false) {}

MoveLogger &MoveLogger::instance() {
    static MoveLogger INSTANCE;
    return INSTANCE;
}

void MoveLogger::addLog(Piece *piece, const std::string &disp, const std::string &prev, const std::string &current) {
    m_logs.push_back({getCurrentMoveCount(), piece, disp, prev, current});
}

int MoveLogger::getCurrentMoveCount() const {
    return (m_logs.size() + 2) / 2;
}

std::vector<Log> MoveLogger::getLogs(Piece *p) {
    std::vector<Log> result;
    for (auto &log: m_logs) {
        if (log.piece == p) result.push_back(log);
    }

    return result;
}

void MoveLogger::render() const {
    if (!m_renderActive) return;
    const auto renderer = SharedData::instance().getRenderer();
    const int w = SharedData::instance().SCREEN_WIDTH;
    const int h = SharedData::instance().SCREEN_HEIGHT;
    SDL_Rect r = {w - w / 4, 0, w / 4, h / 2};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    const int log_gap = r.h / 32;
    const int move_number_gap = r.w / 5;
    const int move_gap = (r.w + move_number_gap) / 2;
    const int line_thickness = 1;
    int i = 0;
    for (auto &log: m_logs) {
        int row = (i + 2) / 2;
        SDL_Rect r0 = {r.x, r.y + log_gap * row + line_thickness * (row - 1), r.w, line_thickness};
        SDL_Rect r1 = {r0.x + move_number_gap, r0.y - log_gap, line_thickness, log_gap};
        SDL_Rect r2 = {r0.x + move_gap, r1.y, r1.w, r1.h};
        int move_x = r1.x + 10;
        int move_y = r1.y - 2;
        if (i % 2 == 0) {
            SDL_RenderFillRect(renderer, &r0);
            SDL_RenderFillRect(renderer, &r1);
            SDL_RenderFillRect(renderer, &r2);
        } else {
            move_x = r2.x + 10;
        }
        MTexture move_number_text(SharedData::instance().getRenderer(),
                                  std::to_string(row), {0, 0, 0});
        move_number_text.render({r0.x, r1.y - 1, 10, 10});
        MTexture move_text(SharedData::instance().getRenderer(), log.disp, {0, 0, 0});
        move_text.render({move_x, move_y, static_cast<int>(log.disp.length() * 10), 13});
        i++;
    }
}

void MoveLogger::toggle() {
    m_renderActive = !m_renderActive;
}