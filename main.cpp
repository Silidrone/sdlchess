#include <iostream>
#include <string>
#include "headers/Game.h"
#include "headers/SharedData.h"

int main(int argc, char *args[]) {
    std::cout << "main.cpp: start chess" << std::endl;

    std::string resources_path = "resources/";
    if (argc > 1) {
        resources_path = std::string(args[1]);
        if (resources_path.back() != '/') {
            resources_path += '/';
        }
    }

    SharedData::instance().setResourcesPath(resources_path);

    Game g;
    g.init();
    g.run();
}
