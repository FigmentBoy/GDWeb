#include "Director.hpp"
#include <emscripten.h>

int main() {
    Director::get()->init();
    emscripten_set_main_loop([](){Director::get()->mainLoop();}, 0, 0);

    return 0;
}