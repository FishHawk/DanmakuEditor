#define GLFW_INCLUDE_NONE

#include "iostream"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Game.hpp"

int main() {
  Game::get_instance().loop();
  return 0;
}
