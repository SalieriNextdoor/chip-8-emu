#include "executer.h"
#include "constants.h"
#include "util.h"
#include <cmath>
#include <cstring>
using namespace emulation;

Executer::Executer(byte *memory) : memory{memory} {}

bool Executer::draw_sprite(ushort operation, byte vx, byte vy) {
  byte height = util::get_nth_nibble(operation, 0);
  byte xshift, yshift;
  ushort pos;
  byte pixel;
  bool changed = false;
  for (ushort i = 0; i < height; i++) {
    for (ushort j = 0; j < SPRITE_WIDTH; j++) {
      xshift = nregister[vx] + j;
      yshift = nregister[vy] + i;
      if ((xshift < screen::W_WIDTH) && (yshift < screen::W_HEIGHT)) {
        // pixel position. the y axis needs to be flipped
        pos = screen::W_WIDTH * (screen::W_HEIGHT - yshift) + xshift;
        // extract each pixel to paint from sprite, big endian order
        pixel = (memory[iregister + i] & (1 << (SPRITE_WIDTH - 1 - j))) == 0 ? 0 : 1;
        if (!changed && pixel == 1 && pixels[pos] == 1)
          changed = true;
        pixels[pos] = pixel ^ pixels[pos];
      }
    }
  }
  return changed;
}

int Executer::execute(ushort operation, Instruction instruction) {
  byte vx = util::get_nth_nibble(operation, 2),
       vy = util::get_nth_nibble(operation, 1);

  bool increment = true;
  short op;
  switch (instruction) {
  case Instruction::RETURN_SUBROUTINE:
    pc = stack[--sp];
    break;
  case Instruction::CLEAR_SCR:
    memset(pixels, 0, screen::W_WIDTH * screen::W_HEIGHT);
    break;
  case Instruction::JUMP:
    pc = util::get_addr(operation);
    increment = false;
    break;
  case Instruction::CALL_SUBROUTINE:
    stack[sp++] = pc;
    pc = util::get_addr(operation);
    increment = false;
    break;
  case Instruction::SKIP_VX_EQUAL_N:
    if (nregister[vx] == util::get_nth_byte(operation, 0))
      increment_pc();
    break;
  case Instruction::SKIP_VX_NEQUAL_N:
    if (nregister[vx] != util::get_nth_byte(operation, 0))
      increment_pc();
    break;
  case Instruction::SKIP_VX_EQUAL_VY:
    if (nregister[vx] == nregister[vy])
      increment_pc();
    break;
  case Instruction::ILOAD_NORMAL:
    nregister[vx] = util::get_nth_byte(operation, 0);
    break;
  case Instruction::IADD_NORMAL:
    nregister[vx] += util::get_nth_byte(operation, 0);
    break;
  case Instruction::LOAD_VY_VX:
    nregister[vx] = nregister[vy];
    break;
  case Instruction::ORLOAD_VY_VX:
    nregister[vx] |= nregister[vy];
    break;
  case Instruction::ANDLOAD_VY_VX:
    nregister[vx] &= nregister[vy];
    break;
  case Instruction::XOR_LOAD_VY_VX:
    nregister[vx] ^= nregister[vy];
    break;
  case Instruction::ADD_VY_VX:
    op = nregister[vx] + nregister[vy];
    nregister[vx] = (byte)op;
    if (op > 0xFF)
      nregister[15] = 0x01;
    else
      nregister[15] = 0x00;
    break;
  case Instruction::SUB_VY_VX:
    op = nregister[vx] - nregister[vy];
    nregister[vx] = (byte)op;
    if (op < 0x0)
      nregister[15] = 0x00;
    else
      nregister[15] = 0x01;
    break;
  case Instruction::SHIFT_VY_RIGHT_VX:
    nregister[vx] = nregister[vy] >> 1;
    break;
  case Instruction::SUB_VX_VY:
    op = nregister[vy] - nregister[vx];
    nregister[vx] = (byte)op;
    if (op < 0x0)
      nregister[15] = 0x00;
    else
      nregister[15] = 0x01;
    break;
  case Instruction::SHIFT_VY_LEFT_VX:
    nregister[vx] = nregister[vy] << 1;
    break;
  case Instruction::SKIP_VX_NEQUAL_VY:
    if (nregister[vx] != nregister[vy])
      increment_pc();
    break;
  case Instruction::ILOAD_INDEX:
    iregister = util::get_addr(operation);
    break;
  case Instruction::DRAW_SPRITE:
    if (draw_sprite(operation, vx, vy))
      nregister[15] = 0x01;
    else
      nregister[15] = 0x00;
    break;
  case Instruction::ADD_INDEX_VX:
    iregister += nregister[vx];
    break;
  case Instruction::STORE_BIN_DEC_VX:
    for (int i = 0; i < 3; i++)
      memory[iregister + i] = (nregister[vx] / ((int)pow(10, 2 - i))) % 10;
    break;
  case Instruction::STORE_VX_RANGE:
    for (int i = 0; i <= vx; i++)
      memory[iregister++] = nregister[i];
    break;
  case Instruction::LOAD_VX_RANGE:
    for (int i = 0; i <= vx; i++)
      nregister[i] = memory[iregister++];
    break;
  case Instruction::DO_NOTHING:
    break;
  }

  if (increment)
    increment_pc();
  return 0;
}
