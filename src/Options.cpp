#include "Options.hpp"
#include "Director.hpp"

void showTriggers(bool visible) {
    glUniform1i(glGetUniformLocation(Director::get()->m_shader->m_id, "visibilities[0]"), visible);
}