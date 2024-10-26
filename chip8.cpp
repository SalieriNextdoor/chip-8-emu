#define GLFW_INCLUDE_NONE
#include "constants.h"
#include "emulator.h"
#include "screen.h"
#include <cstdio>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cerr << "Usage: chip8 FILE" << std::endl;
    return 1;
  }

  emulation::Emulator chip8{argv[1]};
  while (!chip8.run())
    ;

  return 0;
}
