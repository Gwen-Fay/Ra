/**
 *
 * Main function
 *
 * Creates an app and runs it.
 *
 * TODO compile flags to chose which app to run (level editor, game, etc.)
 *
 */

#include "gaem_exampleApp.hpp"
#include "gaem_log.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  LOG_INIT;
  LOG_LEVEL;

  gaem::ExampleApp app{};

  try {
    app.run();
  } catch (const std::exception &e) {
    LOG_ERROR(e.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
