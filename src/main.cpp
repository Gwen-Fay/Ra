#include "exampleApp.hpp"
#include "log.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    gaem::ExampleApp app{};

    try {
        app.run();
    } catch (const std::exception &e) {
        LOG_ERROR(e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
