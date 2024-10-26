#ifndef CHIP8_EXECUTER_H
#define CHIP8_EXECUTER_H

#include "constants.h"
#include "parser.h"

namespace emulation {
using ushort = unsigned short;
using byte = unsigned char;
class Executer {
  byte *memory;
  ushort stack[STACK_SIZE] = {0};
  byte sp = 0;
  byte nregister[NREGISTERS] = {0};
  ushort iregister = 0;
  ushort pc = START_INSTRUCTIONS;

  byte pixels[screen::W_WIDTH * screen::W_HEIGHT / emulation::SPRITE_WIDTH] = {0};

  bool draw_sprite(ushort operation, byte vx, byte vy);

public:
  Executer(byte *memory);
  int execute(ushort operation, Instruction instruction);

  bool inline valid_instruction() const {
    return pc >= START_INSTRUCTIONS && pc < END_INSTRUCTIONS;
  }
  void inline increment_pc() { pc += 2; }
  ushort getpc() const { return pc; };

  byte *getpixels() { return pixels; }
};
}; // namespace emulation

#endif
