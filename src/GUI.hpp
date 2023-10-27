#pragma once

#include "imgui.h"

struct Font {
    ImFont* regular;
    ImFont* large;
    ImFont* giant;

    Font(const char* path);
};

class GUI {
public:
    Font* micross;
    GUI();

    static GUI* get() {
        static GUI* instance = new GUI();
        return instance;
    }
};