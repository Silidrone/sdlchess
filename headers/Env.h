// Added as a possible usage in the future

#ifndef CHESS_ENV_H
#define CHESS_ENV_H

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include "HelperFunctions.h"

class Env {
private:
    std::vector<std::pair<std::string, std::string>> vars;
    bool initialized;

    Env() : initialized(false) {}
public:
    static Env &getInstance() {
        static Env instance;
        return instance;
    }

    Env(Env const &) = delete;

    void operator=(Env const &) = delete;

    void init(const std::string &env_file_path) {
        if(initialized) return;

        std::ifstream file;
        file.open(env_file_path);

        while (file.good()) {
            std::string line;
            std::getline(file, line);

            if (line.empty()) break;

            auto env_pair = HelperFunctions::split(line, '=');
            auto value = env_pair[1];
            int replacement_pos;
            while ((replacement_pos = value.find('$')) != std::string::npos) {
                std::string var_name;
                for (int i = replacement_pos + 1; i < value.size() && (isalpha(value[i]) || value[i] == '_'); i++) {
                    var_name += value[i];
                }
                value.replace(replacement_pos, var_name.length() + 1, getVar(var_name));
            }
            vars.push_back({env_pair[0], value});
        }

        file.close();
        initialized = true;
    }

    std::string getVar(const std::string &key) const {
        for (auto &var: vars) {
            if (var.first == key) {
                return var.second;
            }
        }

        return "";
    }
};

#endif //CHESS_ENV_H
