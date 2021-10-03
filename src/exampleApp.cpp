#include "exampleApp.hpp"

namespace gaem {
void ExampleApp::run() {
    while (window.isRunning()) {
        window.updateWindow();
    }
}
} // namespace gaem
