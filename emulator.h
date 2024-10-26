#ifndef CHIP8_EMULATOR_H
#define CHIP8_EMULATOR_H

#include "constants.h"
#include "executer.h"
#include "parser.h"
#include "screen.h"

namespace emulation {

using byte = unsigned char;
using ushort = unsigned short;
class Emulator {
  byte memory[MEM_SIZE] = {0};

  screen::Screen emuScreen;
  Parser parser;
  Executer executer;

  long sz;

  ushort fetch();

  void processInput(GLFWwindow *window);

public:
  Emulator(const char *filepath);
  int run();
};
} // namespace emulation

#endif
