#ifndef CHIP8_EXECUTOR_H
#define CHIP8_EXECUTOR_H

#include "audiohandler.h"
#include "constants.h"
#include "parser.h"
#include <atomic>

namespace emulation {
using ushort = unsigned short;
using byte = unsigned char;
class Executor {
  byte *memory;
  ushort stack[STACK_SIZE] = {0};
  byte sp = 0;
  byte nregister[NREGISTERS] = {0};
  ushort iregister = 0;
  ushort pc = START_INSTRUCTIONS;
  
  std::atomic<byte> delayTimer{0}, soundTimer{0};
  audio::AudioHandler audio{&soundTimer};

  byte pixels[screen::W_WIDTH * screen::W_HEIGHT / emulation::SPRITE_WIDTH] = {0};

  std::atomic_bool canDraw = true;
  bool draw_sprite(ushort operation, byte vx, byte vy);

public:
  Executor(byte *memory);
  int execute(ushort operation, Instruction instruction, ushort lastKey);

  bool inline valid_instruction() const {
    return pc >= START_INSTRUCTIONS && pc < END_INSTRUCTIONS;
  }
  void inline increment_pc() { pc += 2; }
  ushort getpc() const { return pc; };

  void countdown();
  void interrupt();

  byte *getpixels() { return pixels; }
};
}; // namespace emulation

#endif
