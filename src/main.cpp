#include "Director.hpp"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

int main() {
    Director::get()->init();

    #ifdef EMSCRIPTEN
    emscripten_set_main_loop([](){Director::get()->mainLoop();}, 0, 0);
    #else
    while (!glfwWindowShouldClose(Director::get()->m_window)) {
        Director::get()->mainLoop();
    }
    Director::get()->terminate();
    #endif

    return 0;
}