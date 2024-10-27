#ifndef CHIP8_EMULATOR_H
#define CHIP8_EMULATOR_H

#include "constants.h"
#include "executor.h"
#include "parser.h"
#include "screen.h"

namespace emulation {

using byte = unsigned char;
using ushort = unsigned short;
class Emulator {
  byte memory[MEM_SIZE] = {0};

  screen::Screen emuScreen;
  Parser parser;
  Executor executor;

  long sz;
  ushort cpf, fps;

  void setFonts(const byte *fonts, uint length, ushort startAddr = FONT_START_ADDR);

  ushort fetch();

  void run_cycle();

  void processInput(GLFWwindow *window);

public:
  Emulator(const char *filepath, ushort cpf, ushort fps);
  void run();
};
} // namespace emulation

#endif
