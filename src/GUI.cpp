#include "GUI.hpp"

Font::Font(const char* path) {
    regular = ImGui::GetIO().Fonts->AddFontFromFileTTF(path, 16.f);
    large = ImGui::GetIO().Fonts->AddFontFromFileTTF(path, 32.f);
    giant = ImGui::GetIO().Fonts->AddFontFromFileTTF(path, 64.0f);

    ImGui::GetIO().Fonts->Build();
}

GUI::GUI() {
    micross = new Font("static/fonts/micross.ttf");
}