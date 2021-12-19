/**
 *
 * Main function
 *
 * Creates an app and runs it.
 *
 * TODO compile flags to chose which app to run (level editor, game, etc.)
 *
 */

#include <libgaem/libgaem.hpp>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  LOG_INIT;
  LOG_LEVEL;

  gaem::Application app{};

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
